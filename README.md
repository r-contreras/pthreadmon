
# Manual de Usuario
## Compilar el programa
Para compilar el programa se ha facilitado un Makefile ubicado en la carpeta raíz del proyecto. Se debe abrir una terminal en esta carpeta y escribir el comando `make` para que este realice su trabajo. El ejectuable quedará en la ubicación `bin/pthreadmon`  del directorio raíz del proyecto.

> Nota: También se facilitó un `make clean` para limpiar la carpeta generada para el ejecutable.

Esta solución está implementada con la biblioteca de GTK+, específicamente GTK+3.0 y por lo tanto  no compilará si no se encuentra instalada en su sistema. Para instalar esta biblioteca debe ejecutar en la terminal el siguiente comando:
`sudo apt-get install libgtk-3-dev`
>NOTA: Esto ejemplo es utilizando el manejador de paquetes de Ubuntu, si su sistema tiene otro manejador de paquetes entonces debe reemplazar el `apt-get` por el comando de su manejador correspondiente.

## Utilizar el programa
Para ejectuar el programa nada más es de correrlo desde la terminal. Si se encuentra en el directorio raíz del proyecto, basta con escribir en la terminal `bin/pthreadmon` y esto comenzará la ejecución del programa.

El usuario se encontrará en la ventana principal donde debe seleccionar los tres pokémons para cada jugador clickeando en el maletín y escogiendo tres pokemones. En caso de se haya escogido uno que no era deseado, con el botón de **CLEAR TEAM** podrá volver a escoger los pokémons desde cero.

>NOTA: El orden de los pokémons está determinado por el orden en el que se escogieron los mismos. Si se escoge uno más de una vez se lanzará un warning en pantalla.

También se ha implementado la elección de un nombre para cada jugador pero esto es completamente opcional. Por determinado el nombre de cada jugador o entrenador es `Trainer 1` y `Trainer 2`. Si se desea cambiar, se debe escribir el nombre deseado en el cuadro de texto y precionar **READY**.

Una vez que se quiere comenzar con la simulación, el usuario debe presionar el botón de **VS** que se encuentra en medio de la pantalla.
>NOTA: Si no se han seleccionado 3 pokémons por entrenador, se lanzará  un mensaje en pantalla indicando que se deben seleccionar los tres pokémons por entrenador para comenzar la simulación.

Posteriormente se presentará al usuario la ventana de la simulación en donde se pueden visualizar a los entrenadores con sus pokémons. Para comenzar la simulación se debe presionar el botón de **START**. Esto comenzaría la batalla entre los dos equipos de pokémons. Una vez que finaliza la batalla se le presentará al usuario una pantalla con información del tiempo total de duración de la batalla, así como el tiempo que batalló cada pokémon.
> NOTA: Si en algún momento se desea acabar la simulación, se deberá presionar el botón de **STOP** y esto acabará con la ejecución completa de la simulación.

Cuando la simulación se acaba, se regresa a la ventana principal en donde se podrá iniciar de una la simulación nuevamente con los mismos pokémons y nombres de entrenadores o bien, entrar a los maletines y cambiar los pokémons de cada entrenador, así como el nombre de los mismos.

# Detalles de Implementación
El programa sigue una estructura separada por los elementos de Lógica e Interfaz Gráfica. El componente de Interfaz manda los request a la lógica de manera que el usuario interactúa indirectamente con ella. El `src/main.c` está encargado de inicializar la **Arena** y el **Main window** para comenzar la ejecución del programa.
## Lógica
El componente lógico está divido en los siguientes actores:
- **Arena** `src/controller/arena.c`: Contiene el nombre de los entrenadores, la información del equipo de cada entrenador y está encargada de mandar a correr la batalla entre los pokemones. Este actor requiere del **Pokedex** y **Pokémon** para su funcionamiento.

- **Pokémon**`src/controller/pokemon.c`:  Contiene el manejo de la información del pokémon así como sus estadísticas. Requiere de **Pokedex** para su funcionamiento. También posee el tiempo de batalla del pokémon, así como los métodos de sincronización `pokeball` (mutex) y los métodos para aplicar el daño infligido en contra de otro pokémon.

- **Pokedex**`src/controller/pokedex.c`:  Contiene toda la información acerca de las propiedades de los pokémons: `species_name, hp, current_energy, fast_attack, charged_attack, type_id`. 
## Interfaz Gráfica
El componente de visualización está divido en las siguientes ventanas:
- **Main window**`src/view/main_window.c`: Esta es la ventana principal, de tipo `GtkWindow` y contiene todas las imágenes, labels, botones y los llamados a **Choose Pokemon window** y **Fight window**. Esta ventana tiene interacción limitada con la lógica, pues en esta nada más se pueden ingresar los nombres de los entrenadores. Contiene las señales de llamado a las demás ventanas para interactuar con la lógica.

- **Choose Pokemon window**`src/view/choose_pokemon_window.c`: Esta ventana de tipo `GtkDialog` está encargada de dar una visualización al usuario de los pokémons a elegir para la simulación, tiene la capacidad de agregar y limpiar todos los pokemones del equipo de un entrenador. Contiene botones con imágenes de los pokémons que el usuario puede a su disposición para la simulación. Estos son presentados por medio de un `GtkFlowBox`. También posee otra ventana de pop-up de error por si el usuario ya ha seleccionado un pokémon más de una vez.

- **Fight window**`src/view/fight_window.c`: Esta ventana de tipo `GtkWindow` está encargada de visualizar toda la batalla: el pokémon activo de ambos pokemones, sus barras de vida y energía. Posee los botones que inician la ejecución de la batalla y que la finaliza (en este caso mata todo hilo relacionado a su visualización y ejecución). Una vez acabada la batalla, realiza un llamado a un `GtkDialog` encargado de presentar las estadísticas de la batalla.
# Métodos de Sincronización
Los métodos de sincronización utilizados son los siguientes:
- **battle_modifier** `pthread_mutex_t`: Este método de sincronización está encargado de dar el privilegio de modificar los stats de los pokémons a un único hilo a la vez. Se utiliza para evitar el fallo al modificar quién realizó daño a quién durante la ejecución de la batalla.
- **init_barrier** `pthread_barrier_t`: Este método de sincronización se utiliza al inicio del llamado de los hilos para indicar que la batalla ya comenzó. A más detalle, pone a esperar a que todos los pokémons estén listos para comenzar la batalla.
- **pokeball** `pthread_mutex_t`: Este método de sincronización es utilizado internamente por el actor de pokémon para simular lo que es la pokebola del mismo. Cada vez que esta se abre es para que el pokemon salga a batallar y para ser guardado. Cuando se cierra, el pokemon se encuentra dentro de la pokebola o fuera de ella. Sirve para indicarle al hilo del pokémon que ya se encuentra en batalla ya que salió de su pokebola. Cuando se abre y se cierra, se modifican los tiempos de batalla también.

