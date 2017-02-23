SDK de LibreDTE para C
======================

SDK para realizar la integración con los servicios web de LibreDTE desde C.

Este código está liberado bajo licencia [LGPL](http://www.gnu.org/licenses/lgpl-3.0.en.html).
O sea, puede ser utilizado tanto en software libre como en software privativo.

Dependencias:

	# apt-get install build-essential pkg-config libjson-c-dev libcurl4-openssl-dev

Instalar json-c:

	$ git clone https://github.com/json-c/json-c
	$ cd json-c
	$ sh autogen.sh
	$ ./configure
	$ make
	# make install

Compilar SDK LibreDTE:

	$ make

Compilar ejemplos:

	$ make ejemplos

Ejecutar ejemplos (modificar variables según corresponda):

	$ ./ejemplos/001-generar_dte
	$ ./ejemplos/002-actualizar_estado
