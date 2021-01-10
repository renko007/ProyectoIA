Alumno: Gabriel Araya Godoy
rol: 2904673-5
 
OBSERVACIONES: 
*Hice ssh al centos de la universidad pero no me corrió el código debido a que no reconocía la libreria std que usé.
De todas formas comento que las versión g++ es la 9.3.0 y mi std tiene la versión std=c++14

*Por si acaso adjunto un penalty.cpp para calcular la penalización promedio de las soluciones el cual
debe ser compilado y ejecutado manualmente (la instancia a testear se ingresa en la línea 12).

*Hay una carpeta con las soluciones de los datasets mostrados en el informe de la entrega 2.

Instrucciones:

1- Asignar la instancia a testear.
En la línea 19 de proyecto.cpp colocar entre comillas el nombre de la isntacia a testear.
Sólo se requiere el archivo <instancia>.stu para funcionar correctamente, el cual debe estar
dentro de la carpeta "instances" la cual debe estar en el mismo directorio que proyecto.cpp.

Ejemplo: https://i.gyazo.com/c2b969e04941b202e7a518741e3a90ef.png
Ejemplo instances: https://i.gyazo.com/d3ac20cd37d60524732b8596e4c26cbd.png

2- Compilación:
Para compilar hay que abrir la terminal en la carpeta donde están proyecto.cpp y el makefile. Luego escribir "make".

3- Ejecución:
Para ejecutar escribir en la terminal del paso anterior "make run".
¡IMPORTANTE!: Los resultados quedarán registrados en la carpeta "soluciones" que se encuentra encuentra en
el mismo directorio que proyecto.cpp. El entregable debiese tener la carpeta pero si no llegase a estar esa carpeta, 
entonces es necesario crearla o no se guardarán los resultados.

4- Eliminar el ejecutable:
En la misma terminal, luego de que se haya realizado el paso anterior de resultados, escribir "make clean".
