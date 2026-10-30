# Bitácora — Grupo 9

| Fecha | Evento / problema | Acción o corrección |
|---|---|---|
| 10-09-2026 | Entrega 1: propuesta con celda de carga + HX711 (0–300 N). | Ver `docs/informes/Avance_1_propuesta_y_requerimientos.pdf`. |
| 02-10-2026 | Se decide probar un sensor de presión BMP180 dentro de un guante sellado. | Prueba en banco con LILYGO T-OI Plus por el conector Grove. |
| 02-10-2026 | El cable Grove tenía los colores en otro orden (amarillo = GND). | Hilos identificados por posición en el conector y verificados con multímetro. |
| 02-10-2026 | Error de compilación: faltaba la biblioteca del sensor. | Instalada *Adafruit BMP085 Library* (compatible con BMP180). |
| 02-10-2026 | Lectura correcta: ruido en reposo del orden de ±15 Pa; la presión sube al comprimir el guante. | Se desarrolla la página web y el juego controlado por presión. |
| 20-10-2026 | Esquemático y PCB Rev 1–2 en KiCad. J1 y J2 quedaron contiguos al pasar a la placa. | Zócalos fijados a 22,86 mm (formato D1 mini). |
| 20-10-2026 | DRC: 56 diferencias de paridad con el esquemático. | Huellas reenlazadas por referencia: paridad 0. |
| 30-10-2026 | Revisión docente: pistas por la cara inferior a 0,8 mm. | Rev 3: ruteo completo en B.Cu, DRC 0 errores, Gerbers generados. |
| 30-10-2026 | La guía exige además separación de 0,8 mm, 4 agujeros de 3,2 mm y versión/equipo en serigrafía. | Rev 4: placa de 70 × 70 mm, H1–H4, huella del LED adaptada (pads 1,7 mm), C2 con paso 2,5 mm, bibliotecas con rutas relativas. |
