# Instalación Android y USB OTG

1. Instalar el APK en un teléfono Android que soporte USB Host/OTG.
2. Conectar el adaptador USB-C OTG al teléfono y luego el cable USB del Mega 2560.
3. Abrir la app, mantener `MODO DEMOSTRACIÓN` para probar la interfaz sin hardware, o pulsar `CONECTAR` para solicitar el permiso USB.
4. Aceptar el permiso del dispositivo y esperar `ARDUINO CONECTADO`.

El proyecto contempla puentes ATmega16U2, CH340 y CH341 a través de `usb_serial`, sujeto a la compatibilidad del hardware real. La conexión es local por cable; no requiere Internet. iOS y Flutter Web no están soportados.

Si se parte de un checkout que no contiene el esqueleto Android generado por Flutter, ejecutar una vez `flutter create .` dentro de `app/` y conservar `lib/`, `pubspec.yaml` y el `AndroidManifest.xml` de este repositorio. Luego ejecutar `flutter pub get` y `flutter run`.
