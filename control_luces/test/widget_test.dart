import 'package:flutter_test/flutter_test.dart';
import 'package:control_luces_cockpit/main.dart';

void main() {
  testWidgets('muestra la cabina en modo demostración', (tester) async {
    await tester.pumpWidget(const CockpitApp());
    expect(find.text('MODO DEMOSTRACIÓN'), findsOneWidget);
  });
}
