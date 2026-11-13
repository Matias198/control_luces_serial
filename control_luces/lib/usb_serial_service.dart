import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';

import 'package:usb_serial/usb_serial.dart';

import 'models.dart';

abstract interface class UsbSerialService {
  Stream<UsbConnectionState> get connectionState;
  Stream<VehicleState> get vehicleState;
  Future<List<UsbDeviceInfo>> discoverDevices();
  Future<void> connect(UsbDeviceInfo device);
  Future<void> disconnect();
  Future<void> sendCommand(VehicleState desired);
}

class AndroidUsbSerialService implements UsbSerialService {
  static const int baudRate = 115200;

  final _connection = StreamController<UsbConnectionState>.broadcast();
  final _states = StreamController<VehicleState>.broadcast();
  StreamSubscription<UsbEvent>? _usbEvents;
  UsbPort? _port;
  StreamSubscription<Uint8List>? _input;
  Timer? _reconnectTimer;
  String _incoming = '';
  String? _lastDeviceKey;
  bool _autoReconnect = false;
  bool _connecting = false;
  bool _disposed = false;

  AndroidUsbSerialService() {
    _usbEvents = UsbSerial.usbEventStream?.listen(_usbEventHandler);
  }

  @override
  Stream<UsbConnectionState> get connectionState => _connection.stream;
  @override
  Stream<VehicleState> get vehicleState => _states.stream;

  @override
  Future<List<UsbDeviceInfo>> discoverDevices() async {
    final devices = await UsbSerial.listDevices();
    return devices
        .map((device) => UsbDeviceInfo(
              name: device.productName ?? device.deviceName,
              description:
                  '${device.manufacturerName ?? 'USB'} • VID ${device.vid ?? 0} / PID ${device.pid ?? 0}',
              device: device,
            ))
        .toList();
  }

  @override
  Future<void> connect(UsbDeviceInfo info) async {
    _autoReconnect = true;
    _reconnectTimer?.cancel();
    _reconnectTimer = null;
    await _open(info.device as UsbDevice);
  }

  Future<void> _open(UsbDevice device) async {
    if (_connecting || _disposed) return;
    _connecting = true;
    await _closePort(notify: false);
    _connection.add(UsbConnectionState.waitingPermission);
    try {
      final port = await UsbSerial.createFromDeviceId(device.deviceId);
      if (port == null) throw StateError('No se pudo crear el puerto USB.');
      _connection.add(UsbConnectionState.connecting);
      if (!await port.open()) {
        throw StateError('El puerto USB rechazó la apertura.');
      }
      await port.setDTR(true);
      await port.setRTS(true);
      await port.setPortParameters(
        baudRate,
        UsbPort.DATABITS_8,
        UsbPort.STOPBITS_1,
        UsbPort.PARITY_NONE,
      );
      _port = port;
      _lastDeviceKey = _deviceKey(device);
      _input = port.inputStream?.listen(
        _consumeBytes,
        onError: (_) => _handlePortLost(),
        onDone: _handlePortLost,
      );
      _connection.add(UsbConnectionState.connected);
      await _write('GET');
    } catch (_) {
      await _closePort(notify: false);
      _connection.add(UsbConnectionState.error);
      _scheduleReconnect();
      rethrow;
    } finally {
      _connecting = false;
    }
  }

  void _usbEventHandler(UsbEvent event) {
    if (_disposed || !_autoReconnect) return;
    if (event.event == UsbEvent.ACTION_USB_DETACHED) {
      final device = event.device;
      if (device == null || _lastDeviceKey == _deviceKey(device)) {
        _handlePortLost();
      }
    } else if (event.event == UsbEvent.ACTION_USB_ATTACHED) {
      _scheduleReconnect();
    }
  }

  String _deviceKey(UsbDevice device) =>
      '${device.vid}:${device.pid}:${device.manufacturerName}:${device.productName}';

  void _handlePortLost() {
    if (_disposed || _port == null) return;
    _closePort();
    _connection.add(UsbConnectionState.disconnected);
    _states.add(const VehicleState());
    _scheduleReconnect();
  }

  void _scheduleReconnect() {
    if (!_autoReconnect || _disposed || _reconnectTimer != null) return;
    _reconnectTimer = Timer(const Duration(milliseconds: 500), () async {
      _reconnectTimer = null;
      if (!_autoReconnect || _disposed || _port != null) return;
      try {
        final devices = await discoverDevices();
        if (devices.isEmpty) {
          _scheduleReconnect();
          return;
        }
        final selected = _lastDeviceKey == null
            ? devices.first
            : devices.firstWhere(
                (info) => _deviceKey(info.device as UsbDevice) == _lastDeviceKey,
                orElse: () => devices.first,
              );
        await _open(selected.device as UsbDevice);
      } catch (_) {
        _scheduleReconnect();
      }
    });
  }

  void _consumeBytes(Uint8List bytes) {
    _incoming += utf8.decode(bytes, allowMalformed: true);
    while (_incoming.contains('\n')) {
      final end = _incoming.indexOf('\n');
      final line = _incoming.substring(0, end).trim();
      _incoming = _incoming.substring(end + 1);
      if (line.isNotEmpty) _parseLine(line);
    }
  }

  void _parseLine(String line) {
    final parts = line.split(';');
    if (parts.isEmpty) return;
    if (parts.first == 'ERR') {
      _connection.add(UsbConnectionState.error);
      return;
    }
    if (parts.first != 'STATE' && parts.first != 'ACK') return;
    final values = <String, bool>{};
    for (final part in parts.skip(1)) {
      final pair = part.split('=');
      if (pair.length == 2 && (pair[1] == '0' || pair[1] == '1')) {
        values[pair[0]] = pair[1] == '1';
      }
    }
    // El backend siempre informa los seis campos del estado confirmado.
    const requiredFields = {'low', 'high', 'left', 'right', 'hazard', 'brake'};
    if (!requiredFields.every(values.containsKey)) return;
    _states.add(VehicleState(
      lowBeam: values['low'] ?? false,
      highBeam: values['high'] ?? false,
      leftTurn: values['left'] ?? false,
      rightTurn: values['right'] ?? false,
      hazard: values['hazard'] ?? false,
      brake: values['brake'] ?? false,
      isConfirmed: true,
    ));
  }

  Future<void> _write(String command) async {
    final port = _port;
    if (port == null) throw StateError('Arduino desconectado.');
    await port.write(Uint8List.fromList(utf8.encode('$command\n')));
  }

  @override
  Future<void> sendCommand(VehicleState desired) => _write(
        'SET;low=${desired.lowBeam ? 1 : 0};'
        'high=${desired.highBeam ? 1 : 0};'
        'left=${desired.leftTurn ? 1 : 0};'
        'right=${desired.rightTurn ? 1 : 0};'
        'hazard=${desired.hazard ? 1 : 0};'
        'brake=${desired.brake ? 1 : 0}',
      );

  @override
  Future<void> disconnect() async {
    _autoReconnect = false;
    _reconnectTimer?.cancel();
    _reconnectTimer = null;
    await _closePort();
    _connection.add(UsbConnectionState.disconnected);
  }

  Future<void> _closePort({bool notify = false}) async {
    final input = _input;
    final port = _port;
    _input = null;
    _port = null;
    _incoming = '';
    // Limpiar las referencias antes de cancelar evita que onDone vuelva a
    // entrar en _handlePortLost mientras se está cerrando el puerto.
    await input?.cancel();
    await port?.close();
    if (notify) _connection.add(UsbConnectionState.disconnected);
  }

  void dispose() {
    _disposed = true;
    _autoReconnect = false;
    _reconnectTimer?.cancel();
    _usbEvents?.cancel();
    _input?.cancel();
    _port?.close();
    _connection.close();
    _states.close();
  }
}
