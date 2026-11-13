# Instrucciones para el agente de código — Sistema didáctico de control de luces de automóvil

## 1. Rol y objetivo

Actuá como un agente de desarrollo senior especializado en Flutter, Dart, Arduino C/C++ y comunicación USB serial en Android.

Debés iniciar el desarrollo de un proyecto educativo para una escuela secundaria técnica. El sistema simula el control de iluminación de un automóvil mediante:

- Una aplicación móvil desarrollada con Flutter.
- Una placa Arduino Mega 2560 basada en ATmega2560.
- Una conexión física USB OTG entre un teléfono Android y el Arduino.
- LEDs de distintos colores que representan las luces del vehículo.
- Un interruptor o pulsador físico que simula el pedal de freno.

La aplicación debe presentar una interfaz visual de cabina de automóvil, desde la perspectiva del conductor, y permitir controlar las luces desde elementos gráficos que representen los mandos reales del vehículo.

**No desarrolles un sistema para instalar en un automóvil real.** Es una maqueta didáctica de baja tensión. No debe conectarse al cableado, batería, ECU ni circuitos de iluminación de un vehículo.

## 2. Primera tarea: inspección del entorno

Antes de modificar archivos:

1. Inspeccioná el workspace y determiná si ya existe un proyecto Flutter, un proyecto Arduino o ambos.
2. Identificá versiones instaladas de Flutter, Dart y PlatformIO/Arduino CLI si están disponibles.
3. Conservá los archivos y convenciones existentes cuando sean adecuados.
4. Si el workspace está vacío, creá una estructura clara que permita desarrollar la aplicación Flutter y el firmware Arduino en el mismo repositorio.
5. No sobrescribas archivos de configuración sin inspeccionarlos.
6. Presentá un resumen breve de la estructura encontrada y del plan de implementación.
7. Si falta información sobre el cableado físico, usá las asignaciones propuestas en este documento como valores iniciales configurables, sin afirmar que el circuito ya está conectado.

No inventes componentes que el usuario no haya confirmado. Podés documentar los componentes adicionales necesarios para una conexión eléctrica segura.

## 3. Arquitectura del sistema

La arquitectura debe separar claramente la interfaz, la comunicación y el control físico.

```text
┌────────────────────────────────────────────┐
│              Teléfono Android              │
│                                            │
│  Aplicación Flutter                        │
│  ├── Interfaz de cabina                    │
│  ├── Mandos e indicadores                  │
│  ├── Estado visual del vehículo            │
│  └── Servicio de conexión USB serial       │
└──────────────────────┬─────────────────────┘
                       │ USB-C OTG
                       │ USB serial
┌──────────────────────▼─────────────────────┐
│             Arduino Mega 2560              │
│                                            │
│  ├── Parser de comandos                    │
│  ├── Máquina de estados de iluminación     │
│  ├── Temporización no bloqueante           │
│  ├── Lectura del interruptor de freno      │
│  └── Control de salidas                    │
└──────────────────────┬─────────────────────┘
                       │
             ┌─────────┴─────────┐
             │                   │
       LEDs de maqueta     Pulsador de freno
       (luces del auto)     (entrada física)
```

### 3.1. Responsabilidades de Flutter

Flutter se encarga de:

- Mostrar la cabina y sus controles.
- Permitir activar y desactivar luces.
- Enviar comandos al Arduino.
- Recibir confirmaciones y estados del Arduino.
- Reflejar en pantalla el estado real informado por la placa.
- Informar si el dispositivo está conectado, desconectado o intentando reconectarse.
- Gestionar permisos USB de Android.
- Evitar que el usuario interprete un comando enviado como una activación confirmada antes de recibir la respuesta del Arduino.

### 3.2. Responsabilidades del Arduino

El firmware se encarga de:

- Recibir y validar comandos seriales.
- Mantener el estado de las luces.
- Resolver incompatibilidades entre órdenes.
- Ejecutar intermitencias sin utilizar `delay()`.
- Leer el interruptor físico del freno.
- Controlar las salidas de los LEDs.
- Informar estados y errores a Flutter.
- Inicializar las salidas en estado seguro al encender o reiniciar.

**La lógica temporal de los intermitentes debe ejecutarse en Arduino**, no en Flutter. La desconexión o el cierre de la aplicación no debe dejar una intermitencia dependiente del teléfono.

## 4. Funcionalidades requeridas

### 4.1. Iluminación delantera

- Luz baja: encendido y apagado.
- Luz alta: encendido y apagado.
- No permitir que alta y baja estén activadas simultáneamente, salvo que el diseño de la maqueta justifique expresamente ese comportamiento.
- Los indicadores del tablero deben reflejar el estado confirmado.

### 4.2. Luces de giro

- Giro izquierdo.
- Giro derecho.
- Cada señal debe parpadear con una frecuencia configurable.
- Activar el giro izquierdo debe cancelar el giro derecho y viceversa.
- El parpadeo debe ser no bloqueante, usando `millis()`.

### 4.3. Balizas

- Activar y desactivar ambas señales de giro simultáneamente.
- Las balizas deben tener prioridad sobre las señales individuales.
- Al activar balizas, cancelar el modo de giro individual.
- Al desactivar balizas, no reactivar automáticamente un giro anterior: esperar una nueva orden.

### 4.4. Luz de freno

La luz de freno debe responder principalmente a una entrada física conectada al Arduino.

- Utilizar un pulsador o interruptor para simular el pedal.
- Configurar la entrada con `INPUT_PULLUP` si el circuito se conecta entre el pin y GND.
- Considerar pulsado cuando la entrada esté en LOW.
- Incorporar antirrebote por software.
- Informar el estado del freno a Flutter.
- El control físico debe funcionar aunque el teléfono esté desconectado.

Para pruebas de interfaz puede existir un modo de simulación del pedal desde Flutter, pero debe estar claramente identificado como simulación y no sustituir la entrada física en el modo normal.

### 4.5. Estado de conexión

La aplicación debe distinguir como mínimo:

- Desconectado.
- Dispositivo USB detectado.
- Esperando permiso USB.
- Conectando.
- Conectado.
- Error de comunicación.

Al desconectar el cable, Flutter debe cerrar la sesión serial, cancelar suscripciones y actualizar la interfaz. Debe permitir volver a conectar sin reiniciar la aplicación.

## 5. Interfaz visual Flutter

### 5.1. Concepto visual

Diseñar una interfaz de cabina de automóvil desde la perspectiva del conductor. La aplicación debe parecer un tablero de vehículo y no una pantalla de administración convencional.

Orientación preferida: horizontal (`landscape`).

La interfaz debe adaptarse a diferentes resoluciones y proporciones de pantalla. Evitar posiciones absolutas rígidas que rompan el diseño en otros teléfonos.

### 5.2. Elementos visuales

Incluir:

1. Volante central o ligeramente desplazado hacia la izquierda, según la composición.
2. Tablero de instrumentos detrás del volante.
3. Velocímetro decorativo; no requiere sensor ni medición real.
4. Indicadores de giro izquierdo y derecho.
5. Indicador de luz alta.
6. Indicador de luz baja.
7. Indicador de freno.
8. Mando de luces delanteras.
9. Control de giro izquierdo/derecho inspirado en una palanca.
10. Botón de balizas.
11. Pedal de freno visual para modo de simulación.
12. Indicador de conexión USB.
13. Panel discreto de estado o diagnóstico.

Los elementos deben ser interactivos cuando representen un mando. Los indicadores deben ser visualmente diferentes de los controles.

### 5.3. Estados visuales

- Luz apagada: icono tenue.
- Luz encendida: icono iluminado.
- Giro activo: indicador intermitente sincronizado con el estado informado por Arduino.
- Balizas: ambos indicadores de giro intermitentes.
- Freno activo: indicador rojo visible.
- USB conectado: indicador de conexión positivo.
- USB desconectado: aviso claro, sin aparentar que los comandos se están ejecutando.

No es necesario desarrollar gráficos 3D complejos en la primera iteración. Priorizar una interfaz atractiva, legible, fluida y funcional. Se pueden utilizar SVG, CustomPainter, imágenes o widgets Flutter según convenga. Evitar incorporar recursos visuales con licencias dudosas.

### 5.4. Experiencia de uso

- Los controles deben tener áreas táctiles amplias.
- Mostrar el estado de cada función.
- Evitar enviar repetidamente el mismo comando si el estado solicitado no cambió.
- Mostrar errores de conexión de forma comprensible.
- No bloquear toda la interfaz durante una operación USB.
- La interfaz debe seguir mostrando el último estado conocido, identificado como tal si se pierde la conexión.

## 6. Comunicación USB serial

### 6.1. Plataforma

La conexión USB serial directa se implementará inicialmente para **Android**.

Requisitos:

- El teléfono debe soportar USB Host / OTG.
- Se requiere un adaptador USB-C OTG adecuado.
- El Mega 2560 Rev3 suele utilizar USB tipo B.
- La app debe solicitar el permiso USB al detectar el dispositivo.
- La implementación debe contemplar dispositivos originales y placas compatibles que utilicen diferentes puentes USB-serial, por ejemplo ATmega16U2, CH340 o CH341, según el hardware real.

No implementar USB serial Android mediante Firebase, HTTP ni WebSockets. Es una conexión local directa.

No asumir compatibilidad con iOS ni Flutter Web. Documentar la limitación de plataformas.

### 6.2. Dependencia Flutter

Antes de elegir el paquete:

1. Evaluá paquetes de USB serial disponibles en pub.dev.
2. Revisá mantenimiento, documentación, compatibilidad con versiones actuales de Flutter/Android y soporte para USB Host.
3. Preferí una solución Android USB Host que permita detectar dispositivos, solicitar permisos, abrir el puerto y recibir/enviar bytes.
4. No agregues varias dependencias que resuelvan la misma función.
5. Encapsulá la dependencia detrás de una abstracción propia para poder reemplazarla sin modificar la interfaz ni la lógica de negocio.

Crear una interfaz de servicio similar a:

```dart
abstract interface class UsbSerialService {
  Stream<UsbConnectionState> get connectionState;
  Stream<VehicleState> get vehicleState;

  Future<List<UsbDeviceInfo>> discoverDevices();
  Future<void> connect(UsbDeviceInfo device);
  Future<void> disconnect();

  Future<void> sendCommand(VehicleCommand command);
}
```

Adaptar nombres y modelos al estilo del proyecto existente. No es obligatorio copiar literalmente esta interfaz.

### 6.3. Parámetros seriales

Usar inicialmente:

- Baud rate: `9600` o `115200`, seleccionando un único valor común entre Flutter y Arduino.
- 8 bits de datos.
- Sin paridad.
- 1 bit de parada.
- Terminación de mensajes por salto de línea (`\n`).

Preferir `115200` si la librería y el puente USB-serial funcionan correctamente. Mantener el valor en una constante documentada y compartida conceptualmente por ambos proyectos.

### 6.4. Protocolo de mensajes

Diseñar un protocolo de texto sencillo, delimitado por líneas, de bajo consumo de memoria y fácil de depurar.

El Mega 2560 tiene SRAM limitada. Evitar buffers dinámicos grandes y parsers JSON pesados si no son necesarios.

Propuesta de protocolo inicial:

**Comando de Flutter al Arduino:**

```text
SET;low=1;high=0;left=0;right=0;hazard=0
```

**Confirmación del Arduino:**

```text
ACK;low=1;high=0;left=0;right=0;hazard=0
```

**Estado informado por Arduino:**

```text
STATE;low=1;high=0;left=0;right=0;hazard=0;brake=1
```

**Respuesta de error:**

```text
ERR;code=INVALID_COMMAND
```

Los nombres y campos pueden evolucionar, pero deben quedar documentados y ser idénticos en ambos extremos.

Reglas:

- Cada mensaje termina en `\n`.
- Un mensaje incompleto no debe ejecutarse.
- Limitar el tamaño del buffer serial.
- Descartar mensajes demasiado largos o malformados.
- Validar que los valores sean `0` o `1`.
- No ejecutar comandos desconocidos.
- Evitar concatenar mensajes sin delimitadores.
- Enviar confirmaciones para que Flutter pueda distinguir entre orden enviada y estado aplicado.
- Enviar el estado cuando cambie una entrada física, cuando se aplique una orden o al establecer la conexión.
- No transmitir el estado continuamente sin necesidad.

### 6.5. Sincronización inicial

Al conectar:

1. Flutter detecta el dispositivo.
2. Solicita permiso USB.
3. Abre el puerto serial.
4. Espera el mensaje de inicio o consulta de estado.
5. Arduino informa el estado actual de las luces y el freno.
6. Flutter actualiza toda la interfaz con ese estado.
7. La app queda habilitada para enviar comandos.

No asumir que todos los estados están apagados solamente porque Flutter acaba de iniciar. El estado autoritativo debe provenir del Arduino.

Considerar que algunas placas se reinician al abrir el puerto serial. El firmware debe inicializar las salidas de manera segura y volver a informar el estado después del arranque.

## 7. Firmware Arduino

### 7.1. Organización

Separar el firmware por responsabilidades. Estructura sugerida:

```text
firmware/
└── arduino_mega/
    ├── platformio.ini
    ├── include/
    │   ├── config.h
    │   └── protocol.h
    └── src/
        ├── main.cpp
        ├── lighting_controller.cpp
        ├── lighting_controller.h
        ├── serial_protocol.cpp
        ├── serial_protocol.h
        ├── brake_input.cpp
        └── brake_input.h
```

Si el workspace ya utiliza Arduino IDE, se puede adaptar la estructura sin imponer PlatformIO. La placa objetivo debe ser Arduino Mega 2560.

### 7.2. Asignación inicial de pines

Usar la siguiente asignación como propuesta configurable, no como una conexión confirmada:

| Función | Pin Mega |
|---|---:|
| Luz baja | D22 |
| Luz alta | D23 |
| Giro izquierdo | D24 |
| Giro derecho | D25 |
| Luz de freno | D26 |
| Interruptor físico de freno | D27 |
| Balizas | Lógica interna, sin pin independiente |

Balizas es un modo lógico que activa las dos salidas de giro; no requiere un LED adicional, salvo que el diseño físico agregue un indicador específico.

Mantener todos los pines en `config.h`.

Evitar utilizar pines 0 y 1 para otras funciones porque están asociados al puerto serial principal USB del Mega.

### 7.3. Asignación eléctrica de LEDs

Para LEDs individuales:

- Cada LED debe tener su propia resistencia limitadora.
- No conectar un LED directamente entre un pin y GND sin resistencia.
- Respetar los límites eléctricos de los pines.
- Utilizar una fuente adecuada si se agregan muchos LEDs.
- Compartir GND entre Arduino y la fuente externa cuando el circuito lo requiera.

Para tiras LED, módulos de potencia o cargas que superen la capacidad de un pin, utilizar transistores/MOSFET o drivers apropiados. No alimentar esas cargas desde los pines de Arduino.

La conexión de la entrada de freno con `INPUT_PULLUP` debe realizarse mediante un pulsador entre D27 y GND, salvo que el circuito real requiera otra configuración.

### 7.4. Lógica de iluminación

Implementar una máquina de estados explícita.

Reglas mínimas:

- `low` y `high` son mutuamente excluyentes.
- `left` y `right` son mutuamente excluyentes cuando se opera en modo individual.
- `hazard=1` activa ambas salidas de giro y cancela el giro individual.
- Desactivar balizas apaga ambas señales, salvo que llegue una nueva orden.
- `brake` proviene de la entrada física y se gestiona independientemente.
- El freno no debe ser cancelado por balizas o giros.
- Las salidas deben inicializarse apagadas durante el arranque.
- Los comandos inválidos no deben modificar el estado vigente.

La frecuencia de intermitencia debe poder ajustarse desde una constante, inicialmente alrededor de 1 Hz (por ejemplo, 500 ms encendido y 500 ms apagado).

Usar `millis()` para temporización no bloqueante. No usar `delay()` en la lógica principal.

### 7.5. Entrada física del freno

Implementar:

- Lectura periódica o por mecanismo apropiado.
- Antirrebote por software.
- Detección de cambios de estado.
- Actualización inmediata de la salida de freno.
- Envío de un mensaje `STATE` cuando cambie el estado estable.
- Separación entre estado físico real y simulación de interfaz.

El modo de simulación desde Flutter, si se implementa, debe estar desactivado por defecto y no debe ocultar el estado de la entrada física.

## 8. Modelo de estado compartido

Crear un modelo único y claro para representar las luces.

Ejemplo conceptual:

```dart
class VehicleState {
  final bool lowBeam;
  final bool highBeam;
  final bool leftTurn;
  final bool rightTurn;
  final bool hazard;
  final bool brake;
}
```

El modelo real puede incluir metadatos de conexión, marca temporal o fuente del estado, si resulta útil.

Separar:

- Estado deseado: lo que el usuario solicitó.
- Estado confirmado: lo que Arduino informó.
- Estado físico: lectura real del interruptor de freno.
- Estado de conexión: disponibilidad del canal USB.

No mostrar como confirmado un estado que todavía no fue reconocido por Arduino.

## 9. Organización sugerida de Flutter

Si no existe una arquitectura previa, utilizar una estructura mantenible y sencilla:

```text
app/
├── lib/
│   ├── main.dart
│   ├── app.dart
│   ├── core/
│   │   ├── constants/
│   │   ├── theme/
│   │   └── errors/
│   ├── data/
│   │   ├── models/
│   │   └── protocol/
│   ├── services/
│   │   └── usb_serial/
│   ├── features/
│   │   ├── dashboard/
│   │   │   ├── screens/
│   │   │   ├── widgets/
│   │   │   └── controllers/
│   │   └── connection/
│   └── shared/
│       └── widgets/
├── test/
└── pubspec.yaml
```

La estructura es orientativa. Si el proyecto existente tiene una organización razonable, conservarla.

Utilizar gestión de estado coherente con el proyecto. Si no hay una solución elegida, preferir una alternativa simple y mantenible, evitando complejidad innecesaria para una maqueta escolar.

## 10. Firebase

Firebase no debe formar parte del camino crítico de control entre el teléfono y Arduino.

La primera versión debe funcionar completamente sin Internet.

Si se utiliza Firebase:

- Priorizar Firebase App Distribution para distribuir APK de prueba.
- No almacenar credenciales ni secretos en el código fuente.
- No enviar comandos USB a través de Firebase.
- No introducir autenticación o base de datos si no existe un requisito funcional que lo justifique.
- Documentar claramente que la conexión física se realiza localmente por USB OTG.

## 11. Pruebas requeridas

### 11.1. Pruebas del firmware

Verificar:

1. Encendido y apagado de luz baja.
2. Encendido y apagado de luz alta.
3. Exclusión mutua entre alta y baja.
4. Giro izquierdo intermitente.
5. Giro derecho intermitente.
6. Exclusión entre giros individuales.
7. Balizas simultáneas.
8. Prioridad de balizas.
9. Luz de freno con pulsador físico.
10. Funcionamiento del freno durante balizas.
11. Antirrebote del pulsador.
12. Mensajes seriales inválidos.
13. Mensajes seriales incompletos.
14. Reinicio del Arduino.
15. Desconexión del teléfono durante una intermitencia.

### 11.2. Pruebas de Flutter

Verificar:

1. Detección del Arduino por USB.
2. Solicitud de permiso.
3. Conexión y desconexión.
4. Envío de comandos.
5. Recepción y parseo de `ACK`, `STATE` y `ERR`.
6. Sincronización del estado al conectar.
7. Reconexión después de desconectar el cable.
8. Actualización de indicadores visuales.
9. Respuesta de la interfaz durante la comunicación.
10. Comportamiento correcto al perder conexión.
11. Diseño horizontal en distintas resoluciones.
12. Ausencia de comandos duplicados innecesarios.

### 11.3. Pruebas sin hardware

La aplicación debe poder ejecutarse en un modo de demostración claramente identificado.

En ese modo:

- Simular estados.
- Permitir probar la interfaz.
- No mostrar el modo simulado como una conexión USB real.
- No afirmar que los LEDs físicos están encendidos.

El modo de demostración no debe reemplazar ni interferir con el funcionamiento normal USB.

## 12. Etapas de desarrollo

Trabajar de forma incremental. No intentar implementar toda la aplicación en una sola modificación.

### Etapa 1 — Estructura inicial

- Inspeccionar el workspace.
- Crear o adaptar la estructura Flutter y firmware.
- Configurar la placa Arduino Mega 2560.
- Documentar dependencias y herramientas.

**Criterio de aceptación:** ambos proyectos tienen una estructura ejecutable y documentada.

### Etapa 2 — Firmware mínimo

- Configurar pines.
- Implementar salidas de LEDs.
- Implementar entrada física de freno.
- Implementar intermitencia no bloqueante.
- Implementar protocolo serial.
- Verificar con monitor serial.

**Criterio de aceptación:** el Arduino puede controlar todas las funciones mediante comandos seriales y reportar el freno físico.

### Etapa 3 — Comunicación Flutter-Android

- Investigar y seleccionar paquete USB serial.
- Implementar detección de dispositivo y permisos.
- Implementar conexión, desconexión y recepción serial.
- Implementar parser y envío de comandos.
- Implementar sincronización inicial.

**Criterio de aceptación:** Flutter controla el Arduino desde un teléfono Android mediante USB OTG.

### Etapa 4 — Interfaz de cabina

- Diseñar tablero y volante.
- Incorporar mandos interactivos.
- Incorporar indicadores luminosos.
- Vincular controles al modelo de estado.
- Agregar indicadores de conexión.

**Criterio de aceptación:** la interfaz representa visualmente y controla todas las funciones implementadas.

### Etapa 5 — Integración y pruebas

- Probar todas las combinaciones.
- Corregir inconsistencias.
- Verificar reconexión.
- Probar el modo de demostración.
- Documentar el circuito y el procedimiento de uso.

**Criterio de aceptación:** el sistema funciona de extremo a extremo y sus limitaciones están documentadas.

### Etapa 6 — Distribución

- Generar APK de prueba.
- Configurar Firebase App Distribution si el proyecto lo requiere.
- Documentar instalación, permisos y conexión OTG.

**Criterio de aceptación:** otra persona puede instalar la aplicación y utilizarla con la maqueta siguiendo las instrucciones.

## 13. Calidad y restricciones de implementación

- Utilizar Dart y Flutter para la aplicación.
- Utilizar Arduino C/C++ para el firmware.
- Mantener el código modular y legible para estudiantes de nivel secundario técnico.
- Evitar abstracciones excesivas y dependencias innecesarias.
- No bloquear el loop del Arduino.
- No utilizar `delay()` para los intermitentes.
- No realizar polling agresivo en Flutter.
- No asumir que el teléfono siempre está conectado.
- No asumir que un comando enviado fue ejecutado sin confirmación.
- No usar Firebase como puente para la comunicación USB.
- No exponer controles que aparenten funcionar si no existe conexión, salvo en modo de demostración.
- Mantener pines, baud rate, tiempos y reglas de iluminación en configuraciones documentadas.
- Agregar comentarios en las partes relevantes para que el alumno pueda explicar el funcionamiento.
- No agregar funcionalidades ajenas al alcance sin autorización.

## 14. Documentación que debe producir el agente

Mantener actualizado:

- `README.md` con descripción y arquitectura.
- `docs/CONEXION_ELECTRICA.md` con tabla de pines, resistencias, conexiones y advertencias.
- `docs/PROTOCOLO_SERIAL.md` con mensajes, campos, ejemplos y errores.
- `docs/INSTALACION_ANDROID.md` con requisitos OTG, permisos y pasos de instalación.
- `docs/PRUEBAS.md` con casos de prueba y resultados.
- Instrucciones para compilar y cargar el firmware.
- Instrucciones para ejecutar y compilar Flutter.

No afirmar que una prueba física fue realizada si no se dispuso del hardware. Diferenciar claramente pruebas simuladas, pruebas de compilación y pruebas reales.

## 15. Resultado esperado de la primera intervención

En la primera intervención:

1. Inspeccioná el workspace.
2. Informá qué proyectos y herramientas encontraste.
3. Proponé la estructura definitiva.
4. Identificá los riesgos técnicos o dependencias que deban verificarse.
5. Implementá la estructura inicial y el primer incremento seguro que pueda compilarse.
6. No simules una integración USB completa si todavía no se dispone de un dispositivo físico para probarla.
7. Dejá instrucciones concretas para continuar con la siguiente etapa.

**Prioridad general:** obtener primero un control de luces confiable mediante serial y después construir la interfaz de cabina sobre ese funcionamiento. La estética es importante, pero nunca debe comprometer la claridad del estado ni la confiabilidad del control.
