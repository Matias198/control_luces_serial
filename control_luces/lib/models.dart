enum UsbConnectionState { disconnected, detected, waitingPermission, connecting, connected, error }

class VehicleState {
  const VehicleState({this.lowBeam = false, this.highBeam = false, this.leftTurn = false, this.rightTurn = false, this.hazard = false, this.brake = false, this.blinkOn = false, this.isConfirmed = false});
  final bool lowBeam, highBeam, leftTurn, rightTurn, hazard, brake, blinkOn, isConfirmed;
  VehicleState copyWith({bool? lowBeam, bool? highBeam, bool? leftTurn, bool? rightTurn, bool? hazard, bool? brake, bool? blinkOn, bool? isConfirmed}) => VehicleState(lowBeam: lowBeam ?? this.lowBeam, highBeam: highBeam ?? this.highBeam, leftTurn: leftTurn ?? this.leftTurn, rightTurn: rightTurn ?? this.rightTurn, hazard: hazard ?? this.hazard, brake: brake ?? this.brake, blinkOn: blinkOn ?? this.blinkOn, isConfirmed: isConfirmed ?? this.isConfirmed);
}

class UsbDeviceInfo {
  const UsbDeviceInfo({required this.name, required this.description, required this.device});
  final String name, description;
  final Object device;
}
