---
name: Cockpit Telemetry HUD
colors:
  surface: '#0c141c'
  surface-dim: '#0c141c'
  surface-bright: '#323a43'
  surface-container-lowest: '#070f17'
  surface-container-low: '#141c24'
  surface-container: '#182029'
  surface-container-high: '#232b33'
  surface-container-highest: '#2e363e'
  on-surface: '#dbe3ef'
  on-surface-variant: '#c0c7d4'
  inverse-surface: '#dbe3ef'
  inverse-on-surface: '#29313a'
  outline: '#8a919d'
  outline-variant: '#404752'
  surface-tint: '#9fcaff'
  primary: '#9fcaff'
  on-primary: '#003259'
  primary-container: '#4ca6ff'
  on-primary-container: '#003a66'
  inverse-primary: '#0061a5'
  secondary: '#e9c176'
  on-secondary: '#412d00'
  secondary-container: '#604403'
  on-secondary-container: '#dab36a'
  tertiary: '#8bcff6'
  on-tertiary: '#003549'
  tertiary-container: '#66aad0'
  on-tertiary-container: '#003d55'
  error: '#ffb4ab'
  on-error: '#690005'
  error-container: '#93000a'
  on-error-container: '#ffdad6'
  primary-fixed: '#d2e4ff'
  primary-fixed-dim: '#9fcaff'
  on-primary-fixed: '#001d37'
  on-primary-fixed-variant: '#00497e'
  secondary-fixed: '#ffdea5'
  secondary-fixed-dim: '#e9c176'
  on-secondary-fixed: '#261900'
  on-secondary-fixed-variant: '#5d4201'
  tertiary-fixed: '#c4e7ff'
  tertiary-fixed-dim: '#8bcff6'
  on-tertiary-fixed: '#001e2c'
  on-tertiary-fixed-variant: '#004c69'
  background: '#0c141c'
  on-background: '#dbe3ef'
  surface-variant: '#2e363e'
typography:
  headline-xl:
    fontFamily: Space Grotesk
    fontSize: 32px
    fontWeight: '700'
    lineHeight: 38px
  headline-lg:
    fontFamily: Space Grotesk
    fontSize: 24px
    fontWeight: '700'
    lineHeight: 30px
  headline-md:
    fontFamily: Space Grotesk
    fontSize: 18px
    fontWeight: '600'
    lineHeight: 24px
  body-lg:
    fontFamily: JetBrains Mono
    fontSize: 16px
    fontWeight: '500'
    lineHeight: 24px
  body-md:
    fontFamily: JetBrains Mono
    fontSize: 13px
    fontWeight: '400'
    lineHeight: 18px
  body-sm:
    fontFamily: JetBrains Mono
    fontSize: 11px
    fontWeight: '400'
    lineHeight: 15px
  label-lg:
    fontFamily: JetBrains Mono
    fontSize: 12px
    fontWeight: '700'
    lineHeight: 14px
  label-md:
    fontFamily: JetBrains Mono
    fontSize: 10px
    fontWeight: '600'
    lineHeight: 12px
  label-sm:
    fontFamily: JetBrains Mono
    fontSize: 9px
    fontWeight: '500'
    lineHeight: 10px
  telemetry-display:
    fontFamily: JetBrains Mono
    fontSize: 48px
    fontWeight: '800'
    lineHeight: 48px
spacing:
  gutter: 0.75rem
  gutter-compact: 0.5rem
  margin: 1rem
  margin-landscape: 1.25rem
  space-xs: 0.25rem
  space-sm: 0.5rem
  space-md: 0.75rem
  space-lg: 1rem
  space-xl: 1.5rem
---

## Brand & Style
The visual identity is forged around mission-critical telemetry, avionics glass cockpits, and high-performance racing diagnostic interfaces. Designed exclusively for landscape execution, the interface transforms the display into a high-density, low-latency electronic instrument panel.

Targeted at automotive engineers, performance evaluators, and dynamic lighting simulation technicians, the interface evokes hyper-vigilance, absolute precision, and rapid-scan ergonomic control. The interaction model rejects generic touch patterns in favor of direct digital switchgear, faceted angular readouts, and military-avionics status annunciation.

All system metrics, diagnostic codes, and controls are presented with absolute technical authority. Visual clutter is eliminated: every line segment serves as a structural bezel or calibrated scale index.

## Colors
The palette operates on high-contrast luminescence over a light-absorbing cockpit abyss.
- **Primary (`#4CA6FF` - High Beam Blue / Telemetry Active):** Reserved for primary active telemetry vectors, high-beam indicator arrays, and focused electronic controls. Delivers punch-through contrast against deep night environments.
- **Secondary (`#C5A059` - Calibrated Bronze):** Used for structural threshold lines, precision dial tick-marks, calibration modes, and secondary instrumentation bezels.
- **Tertiary (`#3D84A8` - Marine HUD Cyan):** Applied to inactive tracks, supporting readouts, secondary sensors, and auxiliary status containers.
- **Neutral (`#101820` - Abyss Cockpit Chassis):** The structural core background. Paired with `#182630` for recessed console wells, cluster housings, and modular instrumentation bays.

Functional telemetry status indicators comply with strict ISO standard chromatic signaling:
- **Estado Operativo (`#45C486`):** Actuators engaged, low-beam activation, nominal sensor sync.
- **Advertencia / Precaución (`#F0B44D`):** Thermal thresholds, pressure variances, lighting simulation warnings.
- **Peligro Crítico (`#E74C3C`):** Emergency stops, hazard arrays, brake system fault overrides.
- **Lectura Neutra (`#9AA8B2` / `#F3F5F7`):** Monospace telemetry readouts and dynamic numeric output.

## Typography
Typography is optimized for rapid glance legibility during high-vibration simulation runs.
- **Headlines (`Space Grotesk`):** Aggressive, structural, and technical. Used for subsystem headers, instrument cluster group titles, and primary operational modes. Always set in uppercase with slight tracking expansion (+0.05em).
- **Body & Telemetry (`JetBrains Mono`):** Rigorous monospace rendering ensures numeric data columns never shift position during rapid value cycling (speedometer, RPM, optical flux, millivolt telemetry). All numeric data must use tabular lining.
- **Localization Directives:** All labels, annunciators, and mode selectors are displayed in Spanish (e.g., `VELOCIDAD`, `LUCES LARGAS`, `TELEMETRÍA CRÍTICA`, `PRESIÓN`, `ÁNGULO HAZ`).

## Layout & Spacing
Engineered for dedicated horizontal displays (16:9, 19.5:9, 21:9 landscape orientations). The screen space is organized into three primary operational quadrants:

1. **Banda Lateral Izquierda (Control Switchgear & Entradas):** Fixed 240px dock for lighting configuration switches, beam simulator toggles, and preset banks.
2. **Consola Central (Clúster Principal & Instrumentación Dinámica):** Primary high-density gauge array displaying virtual speedometer, dynamic tachometer, and active ray-tracing light simulation viewports.
3. **Banda Lateral Derecha (Telemetría & Diagnóstico en Tiempo Real):** Real-time stream containing optical flux readouts (lux), bus diagnostics, and thermal monitor tables.

Layout rules:
- Strictly dense 8-pixel and 4-pixel alignment grids.
- Outer borders align with edge margins (`margin-landscape`) to preserve ergonomic thumb zones on physical tablet mounts.
- Inter-module gutters (`gutter`) enforce rigid structural separation without decorative air gaps.

## Elevation & Depth
Depth is produced through milled hardware bevels and layered luminescent states, avoiding floating drop shadows.

- **Base Level (Capa 0 - Chasis):** Deep flat `#101820`. Non-reflective structural housing.
- **Instrument Wells (Capa 1 - Módulos Empotrados):** `#182630` framed with 1px solid borders (`#3D84A8` at 30% alpha). Displays recessed optical depth, mimicking an aircraft instrument panel.
- **Active Annunciation (Capa 2 - Proyección HUD):** Crisp borders illuminated with `#4CA6FF` or `#C5A059`. 
- **Luminance Glow:** High-priority status nodes emit a concentrated inner perimeter glow (`box-shadow: inset 0 0 6px rgba(76, 166, 255, 0.4)` and `0 0 8px rgba(76, 166, 255, 0.35)`). Diffuse drop shadows are prohibited; all depth is optical and co-planar.

## Shapes
Zero-radius architecture (`roundedness: 0`). Curved forms are restricted strictly to functional radial instrumentation (circular tachometers and compass dials).

Structural frames feature 45-degree chamfered corners (`clip-path: polygon(...)`) on primary module envelopes to evoke machined carbon-composite panels and avionics switchboxes. All dividers, buttons, tabular cells, and alert banners possess raw, razor-sharp 90-degree corners.

## Components

### Interruptores de Cabina (Switches & Buttons)
- **Interruptor Basculante (Rocker Switch):** Rectangular sharp-cornered toggle with a dual-state mechanical aesthetic. Resting state: `#182630` with `#3D84A8` 1px border. Active state: `#4CA6FF` fill with black monospace text, accompanied by an integrated 3px illuminated status beacon at the top edge.
- **Botón de Emergencia / Balizas (Hazard Switch):** Dedicated high-visibility toggle with alternating diagonal warning stripes (45-degree `#E74C3C` and `#101820`). Features a 2px permanent safety outline.

### Indicadores ISO 7000 (Instrument Annunciators)
- Status icons (Luz de Cruce, Luz de Carretera, Antiniebla, Freno) rendered in precise 1.5px hairline vector stroke.
- Deactivated: `#182630` icon fill with `#9AA8B2` outlines at 40% opacity.
- Engaged: Immediate transition to solid functional color (`#45C486` for low beam, `#4CA6FF` for high beam) with zero transition delay.

### Display de Telemetría Numérica (Digital Gauge Panels)
- Numeric containers feature a recessed `#0B1117` background with chamfered diagnostic tags displaying engineering units (`KM/H`, `RPM`, `LUX`, `VOLTIOS`).
- Unlit segment ghosting: Inactive numerical matrix segments show at 5% opacity to mimic authentic transflective LCD instrumentation under cockpit illumination.

### Conmutadores Rotativos Didácticos (Rotary Switch Simulators)
- Segmented dial controls featuring calibrated click positions (e.g., `APAGADO`, `AUTO`, `POSICIÓN`, `CRUCE`, `CARRETERA`).
- Current selection marked by a high-intensity `#C5A059` indicator line spanning from outer bezel to switch axis.

### Selectores y Chips de Parámetros (Parameter Chips)
- Rectangular zero-radius tags for setting simulation variables (`ASISTENTE VIRTUAL`, `HAZ ADAPTATIVO`, `MATRIZ LED`).
- Padding: 4px horizontal, 2px vertical. Text: `label-sm` in bold monospace. Active chip features a inverted solid border.

### Campos de Entrada y Calibración (Diagnostic Input Fields)
- Flat rectangular wells with technical cursor brackets (`[` and `]`) framing active value fields instead of standard underline cursors.
- Border shifts to `#4CA6FF` on active terminal focus.