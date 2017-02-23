#ifndef __LIBREDTE_H
#define __LIBREDTE_H

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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define LIBREDTE_URL_SIZE 1000

/* estructura que representa al cliente  */
struct LibreDTE {
	char *hash;
	char *host;
	char *auth;
};

/* estructura que representa la respuesta del cliente */
struct RestResponse {
	int code;
	char *body;
	size_t size;
};

/* prototipos de funciones */
struct LibreDTE *libredte_init(char *hash, char *host);
void libredte_free(struct LibreDTE *cliente);
struct RestResponse *libredte_post(struct LibreDTE *cliente, char *api, char *data);
struct RestResponse *libredte_get(struct LibreDTE *cliente, char *api);
void rest_response_free(struct RestResponse *response);

#endif /* __LIBREDTE_H */
