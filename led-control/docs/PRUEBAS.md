# Pruebas

## Verificables sin hardware

- `pio run` debe compilar el firmware Mega 2560.
- La app debe iniciar en modo demo y permitir alternar baja, alta, giros, balizas y freno visual.
- El modo demo debe estar rotulado y no debe presentarse como USB conectado.
- La UI debe permanecer utilizable en orientación horizontal.

## Verificación con hardware pendiente

Con un Mega y la maqueta conectados, probar: exclusión alta/baja; alta y baja
combinadas por separado con freno activo; exclusión izquierda/derecha; prioridad
y apagado de balizas; intermitencia sin bloquear; freno con rebote; mensajes
inválidos; reinicio; desconexión durante una intermitencia; reconexión y
sincronización con `GET`.

No se afirma aquí una prueba física: en este entorno no se dispuso de Arduino ni teléfono Android OTG.
