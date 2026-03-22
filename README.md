# TP1-Rendimiento

# Informe: Uso de Benchmarks para la Selección de Hardware

## 1. Introducción

Los benchmarks son herramientas que permiten medir el rendimiento de distintos componentes de una PC para un determinado trabajo. Utilizar benchmarks de terceros permite tomar decisiones objetivas al momento de elegir hardware, ya que simulan cargas de trabajo reales.

El rendimiento de los computadores esta relacionado con el tiempo que tarda en ejecutar programas. Es inversamente proporcional al tiempo, es decir, mientras mas tiempo tarda, menor el rendimiento.

---

## 2. Listado de Benchmarks

Gaming / GPU
- Unigine Superposition: Mide rendimiento gráfico (FPS) en escenarios realistas.
- 3DMark: Benchmark estándar con tests modernos como DirectX 12.

CPU (general)
- Geekbench: Mide rendimiento single-core y multi-core. Bueno para comparar CPUs rápido.
- Cinebench: Evalúa CPU usando renderizado real (multinúcleo).

Programación / Desarrollo
- build-linux-kernel: Mide tiempo de compilación del kernel de Linux.
- Phoronix Test Suite: Suite completa de benchmarks para Linux.

Navegador / Web
- Speedometer: Simula uso de aplicaciones web modernas.
- WebXPRT: Mide rendimiento en tareas web reales (IA, imágenes, etc.).

Almacenamiento
- CrystalDiskMark: Mide velocidad de lectura/escritura de discos.
- ATTO Disk Benchmark: Evalúa rendimiento de almacenamiento en distintos tamaños de archivo.

Sistema completo
- PCMark: Simula uso diario del sistema (oficina, navegación, multimedia).

## 3. Tareas habituales

Las tareas consideradas en este análisis son:

- Gaming  
- Uso de navegador (browser)  
- Compilación y ejecución de programas  

---

## 3. Selección de benchmarks

Se seleccionaron benchmarks representativos de cada tipo de tarea:

| Tarea | Benchmark recomendado | ¿Qué mide? | Justificación |
|------|---------------------|-----------|--------------|
| Gaming | Unigine Superposition | Rendimiento gráfico (FPS) | Simula cargas reales de videojuegos |
| Browser | Speedometer / JetStream | Rendimiento en JavaScript | Representa uso de aplicaciones web |
| Compilación | build-linux-kernel (Phoronix) | Tiempo de compilación | Representa workloads de desarrollo |
| Ejecución de programas | Geekbench | Rendimiento CPU general | Evalúa cargas mixtas |

---

## 4. Benchmark de compilación

El benchmark utilizado es:

https://openbenchmarking.org/test/pts/build-linux-kernel-1.15.0

Este benchmark mide el tiempo necesario para compilar el kernel de Linux.

### Características:
- Uso intensivo de CPU multinúcleo  
- Dependencia de memoria y caché  
- Escalabilidad parcial (no completamente paralelizable)  

---

## 5. Rendimiento de procesadores

| Procesador | Tiempo estimado | Version
|-----------|----------------|------------|
| Intel Core i5-13600K | ~83 +/-3 segundos | 1.16.x |
| AMD Ryzen 9 5900X | ~97 +/-6 segundos | 1.16.x | 
| AMD Ryzen 9 7950X | ~52 +/-3 segundos | 1.16.x |

---

## 6. Aceleración (Speedup)

Se utiliza la fórmula:

Speedup = Rendimiento_Mejorado / Rendimiento_Original

### Comparación con Ryzen 9 5900X:

Speedup = 97 / 52 ≈ 1.86

### Comparación con i5-13600K:

Speedup = 83 / 52 ≈ 1.60

---

# Parte 2 - Profiling

El profiling es una tecnica para medir tiempos de ejecución, tiempos que tarda cada función y hasta cuantas veces se llama a una función.

Una herramienta para realizar estas mediciones es ```gprof```.

Para utilizarla se llevo a cabo los siguientes pasos:

### 1 - Compilar con profiling utilizando el comando:
```gcc -Wall -pg test_gprof.c test_gprof_new.c -o test_gprof```

![alt text](<images/Screenshot from 2026-03-22 17-35-03.png>)

![alt text](<images/Screenshot from 2026-03-22 17-34-33.png>)

### 2 - Ejecucion del programa

```./test_gprof```

![alt text](<images/Screenshot from 2026-03-22 17-35-32.png>)

se genera elarchivo "gmon.out", que contiene los datos de la ejecución.

### 3 - Analisis con gprof

```gprof test_gprof gmon.out > analysis.txt```

![alt text](<images/Screenshot from 2026-03-22 17-36-04.png>)

![alt text](<images/Screenshot from 2026-03-22 17-36-30.png>)

![alt text](<images/Screenshot from 2026-03-22 17-36-56.png>)

El analisis muestra que el tiempo de ejecucion esta distribuido en las funciones `func2`, `new_func1` y `func1`.

La función `new_func1` es la que más tiempo consume de forma individual, representando aproximadamente el 34.22% del tiempo total de ejecución. Luego, se encuentra `func2`, con un 33.84%, por lo que ambas podrían considerarse los cuellos de botella del programa.

Se observa que `main` no consume tiempo, `func2` y `new_func1` distribuyen su tiempo a su propia función, mientras que `func1` consume su tiempo y lo distribuye con el llamado a `new_func1`.
