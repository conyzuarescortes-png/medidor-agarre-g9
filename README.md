# Sistema de medición de agarre para rehabilitación — Grupo 9

Proyecto de integración del curso **Electrónica Biomédica**, Universidad del Desarrollo (2026).

**Integrantes:** Martina Casas Del Valle, Catalina Gregoire, María José Polanco, Constanza Villanueva y Constanza Zuares.
**Profesores:** Wladimir Araya, Alejandro Rebolledo, Nicolás Walker.

Prototipo educativo que mide la variación de presión dentro de un recipiente flexible sellado (guante) mediante un sensor **BMP180**, procesada por un **ESP32-C3** (módulo LILYGO T-OI Plus). El ESP32-C3 sirve una página web por WiFi con el valor instantáneo, el máximo y un juego controlado por la presión. **No tiene finalidad diagnóstica** y la variable reportada es presión (kPa), no fuerza.

## Estado

| Parte | Estado |
|---|---|
| Esquemático y PCB (Rev 4, 30-10-2026) | Diseño terminado: pistas de 0,8 mm en B.Cu, separación 0,8 mm, 4 agujeros M3, 70 × 70 mm |
| Gerbers Rev 4 | Pendiente de exportar a `electronica/outputs/gerbers` |
| Firmware | Lectura del BMP180, puesta a cero, máximo, página web y juego. Pendiente: pulsadores y LED de la PCB |
| Pruebas | Cualitativas (respuesta a la compresión). Pendiente: caracterización |

## Estructura

| Ruta | Contenido |
|---|---|
| `docs/informes` | Avance 1 e informe de la Parte II (Word y PDF) |
| `docs/bitacora.md` | Registro fechado de cambios, problemas y correcciones |
| `docs/datasheets` | Enlaces a hojas de datos |
| `electronica/kicad` | Proyecto editable de KiCad 10 (`.kicad_pro`, `.kicad_sch`, `.kicad_pcb`), BOM y LEEME |
| `electronica/libraries` | Biblioteca propia: `G9.kicad_sym` (símbolos) y `G9.pretty` (huella LED adaptada) |
| `electronica/outputs` | BOM, vistas y archivos de fabricación (Gerbers y taladrado) |
| `firmware/medidor_agarre_g9` | Código Arduino del ESP32-C3 (la página web está embebida en el `.ino`) |

## Abrir el proyecto de KiCad

1. Descargar el repositorio completo (**Code → Download ZIP**) y descomprimirlo.
2. Abrir `electronica/kicad/medidor_agarre_g9.kicad_pro` con KiCad 10.
3. Las bibliotecas propias se cargan solas desde `electronica/libraries` (rutas relativas en `sym-lib-table` y `fp-lib-table`). El resto de las huellas y los modelos 3D son los oficiales de KiCad.

## Compilar y cargar el firmware

1. Arduino IDE 2 con el soporte de placas **esp32** (Espressif).
2. Instalar la biblioteca **Adafruit BMP085 Library** (con sus dependencias).
3. Placa: **ESP32C3 Dev Module**; *USB CDC On Boot: Disabled*; puerto COM de la placa.
4. Abrir `firmware/medidor_agarre_g9/medidor_agarre_g9.ino` y cargar (botón **Upload**).
5. Conectarse a la red WiFi **Agarre-G9** (clave `12345678`) y abrir `http://192.168.4.1`.

## Conexiones principales

| Señal | ESP32-C3 | Conector J3 (BMP180) |
|---|---|---|
| SDA | GPIO19 | pin 4 (vía R1 100 Ω) |
| SCL | GPIO18 | pin 3 (vía R2 100 Ω) |
| 3V3 | 3V3 | pin 1 (VIN) |
| GND | GND | pin 2 |
| LED de umbral | GPIO4 | — |
| Pulsador CERO / INICIO-STOP | GPIO6 / GPIO7 | — |

## Licencia

Ver `LICENSE`.
