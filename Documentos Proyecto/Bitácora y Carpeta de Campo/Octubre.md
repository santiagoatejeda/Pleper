# 🗓️ Bitácora - Octubre 2025

## 📅 1️⃣ Semana (Lunes 29 - Viernes 3)


-  Santiago Tejeda: esta semana me dediqué a corroborar por qué los pulsos de la baldosa no llegaban a cargar el banco de capacitores, dentro de todas las razones una de las que me cuestioné era la falta de un capacitor de filtrado en la salida de la placa reciticadora, conectada en parallo, siendo esta conectada en paralelo, pero luego al probarlo en la baldosa dejó de dar tensión a la salida. Lo que terminó en tener que desoldarlo y optar por otra solución.

-  Mauricio Blasco: Diseñe las publicaciones de Instagram de la palca rectificadora y el banco de capacitores, además de diseñar la portada triple de la publicación en Canva. También diseñe el modelo 3d de los topes en Autocad y los pase al formato para imprimirlos en el CURA. Soldé los componentes que adquirimos en la placa rectificadora anteriormente hecha.

-  Victor Raul Broncano: Probe la continuidad del banco de capacitores, agujerie la placa y solde los componentes. Armamos la estractura de la baldosa, comprobando su resitencia y como reacciona con las personas caminando por encima.

-  Ignacio Garcia Louzan: Durante esta semana reescribí casi todo el front-end del sitio PLEPER. Implementé un sidebar exclusivo para móviles, transiciones suaves, efectos de fade in/fade out, fondo animado y una sección de sponsors con rotación automática cada 5 segundos. También corregí el script del menú que aparece en desktop al scrollear hacia arriba. Hubo inconvenientes de centrado y de animaciones que desaparecían de golpe, pero se resolvieron.

---

## 📅 2️⃣ Semana (Lunes 6 - Viernes 10)


-  Santiago Tejeda: esta semana me dediqué a buscar el motivo por el cuál el banco de capacitores no cargaba. Llegué a la conclusión de que la constante de tiempo del capacitor de almacenamiento principal (10,000µF) excede en dos órdenes de magnitud la duración del pulso piezoeléctrico (20ms), resultando en una eficiencia de captura menor al 5%. Para resolver esto, se implementó un sistema de dos etapas capacitivas con un capacitor de acoplamiento (470µF) que presenta una constante de tiempo compatible con la duración del pulso. Denominandola "circuito de bombeo de carga capacitiva".

-  Mauricio Blasco: Esta semana complete el borde de caucho que cubre los costados de la baldosa, además de hacer un corte para que pasen los cables e intentar pegar con cemento de contacto la alfombra antideslizante a la baldosa, lo cual fracasamos ya que estaba ya seco el cemento de contacto, por lo que la despegamos.

- Victor Raul Broncano: Ir a comprar al Easy el tubo de PVC y recortarlo para facilitar su transporte. También contactar con Elemon e ir a su sede en Villa Urquiza para la abonacion de distintos componentes.

- Ignacio Garcia Louzan: Esta semana incluí el diseño de la caja donde iría todo el circuito y la pared donde se colocaría el cartel de emergencia.

---

## 📅 3️⃣ Semana (Lunes 13 - Viernes 17)


-  Santiago Tejeda: esta semana me dediqué a desoldar los pines macho-macho de la Raspberry Pico W, que estaban soldados al revés, aprovechando que teníamos las prácticas profesionlizantes en Newton SRL, nos brindaron uso de su equipo de microscopios y capacitación para soldar/desoldar estaño de la forma correcta, que me facilitó la operación. Luego empecé a investigar sobre los componentes para el prototipado del nuevo circuito de retroalimentación de bombeo de carga capacitiva.

-  Mauricio Blasco: Esta semana me encargue de medir la tensión tanto en la salida del circuito como en la salida de los grupos de piezoeléctricos, midiendo con uno, dos, tres y cuatro topes en un grupo y con un tope en cada esquina.

-  Ignacio Garcia: Esta semana hubieron varios problemas. Primero supimos que la bateria Lipo nunca iba a aguantar la alimentacion de pi pico 2W debido a que la bateria Lipo podia llegar a alimentar hasta 4,1V mientras que la pi pico 2W necesitaba 5V para una buena alimentacion. Basicamente, con la lipo podiamos prender el display pero ya cosas como la parte web le exigian de mas. Ademas, quisimos reemplazar la bateria por una de mayor voltaje pero el modulo de carga empezo a tirar errores de falso contacto, cosa que retrocedio el avance que estabamos llevando. Al final cambiamos la bateria y su modulo de carga por una bateria portatil que asegura la alimentacion de la pico y al mismo tiempo no requierede un modulo de carga.

- Victor Raul Broncano: Compra de segundo tacho de cemento de contacto, recorte y pegado de la alfombra que recubre la baldosa de caucho y la base de madera.  

---

## 📅 4️⃣ Semana (Lunes 20 - Viernes 24)

 
-  Santiago Tejeda: esta semana me dedique a investigar sobre el armado del circuito de retroalimentación para solucionar el problema de que los pulsos de la baldosa no logran ser captados por el banco de capacitores, ya que al tener una alta capacitancia y el tener un pulso de tan corto tiempo (20ms) los capacitores no logran captarlo. La solución que encontré fue una implementación de un circuito de bombeo de carga capacitiva que utiliza un capacitor pre-cargado como colchón de voltaje para maximizar la eficiencia en la captura de pulos piezoeléctricos. Logré armar el prototipo del circuito que, lentamente, carga el capacitor de 470 uF el cual proporciona la base de voltaje que posteriormente se le van a sumar el de los pulsos y así cargar el capacitor de 10.000uF, el cual daría por finalizado la etapa de carga de la baldosa. (Actualización) La etapa no terminó funcionano por falta de potencia de los pulsos, por lo que nunca iban a llegar a cargar el capacitor de 10.000 uF, por lo que decidimos en tratar de armar otra baldosa cambiando la configuración del cableado, en vez de todos en paralelo para mayor corriente, lo cambiamos a todos en serie para sumar pico de voltaje útil.

-  Mauricio Blasco: Esta semana me encargue de medir el tiempo de los pulsos generado por los piezoeléctricos con el objetivo de saber si el problema de la carga de los capacitores podría ser por la corta duración de estos, lo cual terminamos confirmando. También me encargue de comprar algunos componentes que requeríamos para seguir avanzando, los cuales eran 2 resistencias de 4,7 Mohm y dos tiras de 40 pines. Tambien medí la corriente en la salida del circuito tanto sin topes como con uno, dos, tres y cuatro topes. Por ultimo actualize con algunas mejoras el banner del proyecto para luego enviarlo.

-  Ignacio Garcia: Durante esta semana documente gran parte de lo que hice hasta el momento.

- Victor Raul Broncano: Compra y lijado de grampas para mantener el fijo verticalmente el tubo de PVC. En este tubo van a pasar los cables que iran dirigido a la caja de placas. Tambien repare las conexiones desoldadas de los sensores piezoelectricos

- Victoria Josefina Baza: Esta semana ayudé a medir el tiempo de los pulsos generados por los piezoeléctricos. También diseñe modelos 3D de los PCB de las placas que habíamos hecho; la placa de rectificación y la placa de filtrado para poder visualizarlas en FreeCAD, y mostrar una vista conceptual de las mismas.

---

## 📅 5️⃣ Semana (Lunes 27 - Viernes 31)

-  Mauricio Blasco: Me encargue del pensado, diseño, edicion y guionado del proyecto, ademas de diseñar el STL para poder imprimir los 16 topes necesarios, despues los imprimios en el colegio. Me contacte con Tasker Consultores para poder obtener su ayuda para obtener un cartel de emergencia que requeriamos. Me encargue tambien de la grabacion de videos que utilizariamos posteriormente para el video.
-  Victoria Josefina Baza: Esta semana estuve trabajando en el Manual de Usuario, estuve recopilando la información necesaría para poder completarlo según los requisitos que se pedían. Me enfoqué en lo que corresponde a la instalación de la baldosa y su manipulación, ya que como tal no es necesario utilizar una aplicación para visualizar el funcionamiento de la baldosa. Terminé la documentación y la envié el Viernes 31. También estuve diseñando la baldosa en AutoCAD para tener una vista conceptual del diseño de esta, respetando las medidas originales para poder completar las documentaciones y utilizar las vistas de manera didáctica. Por eso saqué también las capturas de pantalla de la baldosa para poder agregarlas a la documentación. Tambien ayude con la grabacion de videos posteriormente utilizados en el video, y grabe el inicio y final del este mismo.
