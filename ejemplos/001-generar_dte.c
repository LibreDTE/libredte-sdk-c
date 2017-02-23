/**
 * LibreDTE
 * Copyright (C) SASCO SpA (https://sasco.cl)
 *
 * Este programa es software libre: usted puede redistribuirlo y/o modificarlo
 * bajo los términos de la GNU Lesser General Public License (LGPL) publicada
 * por la Fundación para el Software Libre, ya sea la versión 3 de la Licencia,
 * o (a su elección) cualquier versión posterior de la misma.
 *
 * Este programa se distribuye con la esperanza de que sea útil, pero SIN
 * GARANTÍA ALGUNA; ni siquiera la garantía implícita MERCANTIL o de APTITUD
 * PARA UN PROPÓSITO DETERMINADO. Consulte los detalles de la GNU Lesser General
 * Public License (LGPL) para obtener una información más detallada.
 *
 * Debería haber recibido una copia de la GNU Lesser General Public License
 * (LGPL) junto a este programa. En caso contrario, consulte
 * <http://www.gnu.org/licenses/lgpl.html>.
 */

/**
 * Ejemplo que muestra los pasos para:
 *  - Emitir DTE temporal
 *  - Generar DTE real a partir del temporal
 *  - Obtener PDF a partir del DTE real
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-23
 */

#include "LibreDTE.h"

char *dte_crear()
{
	json_object *dte = json_object_new_object();
	json_object *TipoDTE = json_object_new_int(33);
	json_object *IdDoc = json_object_new_object();
	json_object *Encabezado = json_object_new_object();
	json_object_object_add(IdDoc, "TipoDTE", TipoDTE);
	json_object_object_add(Encabezado, "IdDoc", IdDoc);
	json_object_object_add(dte, "Encabezado", Encabezado);
	json_object *Emisor = json_object_new_object();
	json_object *RUTEmisor = json_object_new_string("76192083-9");
	json_object_object_add(Emisor, "RUTEmisor", RUTEmisor);
	json_object_object_add(Encabezado, "Emisor", Emisor);
	json_object *Receptor = json_object_new_object();
	json_object *RUTRecep = json_object_new_string("66666666-6");
	json_object_object_add(Receptor, "RUTRecep", RUTRecep);
	json_object *RznSocRecep = json_object_new_string("Persona sin RUT");
	json_object_object_add(Receptor, "RznSocRecep", RznSocRecep);
	json_object *GiroRecep = json_object_new_string("Particular");
	json_object_object_add(Receptor, "GiroRecep", GiroRecep);
	json_object *DirRecep = json_object_new_string("Santiago");
	json_object_object_add(Receptor, "DirRecep", DirRecep);
	json_object *CmnaRecep = json_object_new_string("Santiago");
	json_object_object_add(Receptor, "CmnaRecep", CmnaRecep);
	json_object_object_add(Encabezado, "Receptor", Receptor);
	json_object *Detalle = json_object_new_array();
	json_object *Item = json_object_new_object();
	json_object *NmbItem = json_object_new_string("Producto 1");
	json_object_object_add(Item, "NmbItem", NmbItem);
	json_object *QtyItem = json_object_new_int(2);
	json_object_object_add(Item, "QtyItem", QtyItem);
	json_object *PrcItem = json_object_new_int(1000);
	json_object_object_add(Item, "PrcItem", PrcItem);
	json_object_array_add(Detalle, Item);
	json_object_object_add(dte, "Detalle", Detalle);
	return (char*)json_object_to_json_string(dte);
}

void dte_guardar_pdf(char *file, char* data, size_t size)
{
	size_t i;
	FILE *fd = fopen(file, "wb");
	if (!fd) {
		fprintf(stderr, "Error al guardar el PDF\n");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<size; i++) {
		fwrite(data+i, sizeof(char), 1, fd);
	}
	fclose(fd);
}

int main()
{
	char *url = "https://libredte.cl";
	char *hash = "";
	char recurso[LIBREDTE_URL_SIZE];
	struct LibreDTE *cliente;
	struct RestResponse *emitir, *generar, *generar_pdf;
	cliente = libredte_init(hash, url);
	/* crear dte temporal  */
	emitir = libredte_post(cliente, "/dte/documentos/emitir", dte_crear());
	if (emitir->code != 200) {
		fprintf(stderr, "Error al emitir DTE temporal: %s\n", emitir->body);
		return EXIT_FAILURE;
	}
	/* crear dte real */
	generar = libredte_post(cliente, "/dte/documentos/generar", emitir->body);
	if (generar->code != 200) {
		fprintf(stderr, "Error al generar DTE real: %s\n", generar->body);
		return EXIT_FAILURE;
	}
	/* obtener pdf */
	json_object *dte_emitido = json_tokener_parse(generar->body);
	json_object *dte_emitido_dte = json_object_object_get(dte_emitido, "dte");
	json_object *dte_emitido_folio = json_object_object_get(dte_emitido, "folio");
	json_object *dte_emitido_emisor = json_object_object_get(dte_emitido, "emisor");
	sprintf(
		recurso,
		"/dte/dte_emitidos/pdf/%d/%d/%d",
		json_object_get_int(dte_emitido_dte),
		json_object_get_int(dte_emitido_folio),
		json_object_get_int(dte_emitido_emisor)
	);
	generar_pdf = libredte_get(cliente, recurso);
	if (generar_pdf->code != 200) {
		fprintf(stderr, "Error al generar DTE real: %s\n", generar_pdf->body);
		return EXIT_FAILURE;
	}
	dte_guardar_pdf("documento.pdf", generar_pdf->body, generar_pdf->size);
	/* liberar cliente, respuestas y salir  */
	rest_response_free(emitir);
	rest_response_free(generar);
	rest_response_free(generar_pdf);
	libredte_free(cliente);
	return EXIT_SUCCESS;
}
