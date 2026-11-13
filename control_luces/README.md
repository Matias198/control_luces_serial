# App Flutter de control de luces

Aplicación Android para controlar el firmware Arduino Mega 2560 por USB OTG. Inicia en modo demostración y solo presenta estados físicos como confirmados después de recibir `ACK`/`STATE`.

```bash
flutter pub get
flutter run
flutter build apk --release
```

Requiere Android con USB Host/OTG. Si falta el esqueleto generado por Flutter, ejecutar `flutter create .` dentro de este directorio conservando `lib/`, `pubspec.yaml` y el `AndroidManifest.xml`.
