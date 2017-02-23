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

#include "LibreDTE.h"

size_t rest_response_write(void *ptr, size_t size, size_t nmemb, struct RestResponse *response);
struct RestResponse *rest_response_init();

/**
 * Función que crea e inicializa el cliente
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
struct LibreDTE *libredte_init(char *hash, char *host)
{
	struct LibreDTE *cliente = (struct LibreDTE*)calloc(1,sizeof(struct LibreDTE));
	cliente->hash = (char *)malloc(strlen(hash)+1);
	cliente->host = (char *)malloc(strlen(host)+1);
	cliente->auth = (char *)malloc(strlen(hash)+3);
	if (cliente->hash == NULL || cliente->host == NULL || cliente->auth == NULL) {
		fprintf(stderr, "malloc() falló!\n");
		exit(EXIT_FAILURE);
	}
	strcpy(cliente->hash, hash);
	strcpy(cliente->host, host);
	sprintf(cliente->auth, "%s:X", cliente->hash);
	return cliente;
}

/**
 * Función que libera el cliente
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
void libredte_free(struct LibreDTE *cliente)
{
	free(cliente->hash);
	free(cliente->host);
	free(cliente->auth);
	free(cliente);
}

/**
 * Función que realiza una consulta por POST
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
struct RestResponse *libredte_post(struct LibreDTE *cliente, char *api, char *data)
{
	char recurso[LIBREDTE_URL_SIZE];
	struct RestResponse *response = rest_response_init();
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	response->code = -1;
	if (!curl) {
		fprintf(stderr, "curl_easy_init() falló!\n");
		exit(EXIT_FAILURE);
	}
	sprintf(recurso, "%s/api%s", cliente->host, api);
	curl_easy_setopt(curl, CURLOPT_URL, recurso);
	curl_easy_setopt(curl, CURLOPT_USERPWD, cliente->auth);
	if (data != NULL) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	}
	if (recurso[4]=='s') {
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rest_response_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() falló!\n");
		exit(EXIT_FAILURE);
	}
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->code);
	curl_easy_cleanup(curl);
	return response;
}

/**
 * Función que realiza una consulta por GET
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
struct RestResponse *libredte_get(struct LibreDTE *cliente, char *api)
{
	return libredte_post(cliente, api, NULL);
}

/**
 * Función que inicializa la respuesta del servicio web
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
struct RestResponse *rest_response_init()
{
	struct RestResponse *response = (struct RestResponse*)malloc(sizeof(struct RestResponse));
	response->size = 0;
	response->body = (char *)malloc(response->size+1);
	if (response->body == NULL) {
		fprintf(stderr, "malloc() falló!\n");
		exit(EXIT_FAILURE);
	}
	response->body[response->size] = '\0';
	return response;
}

/**
 * Función que libera la respuesta del servicio web
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
void rest_response_free(struct RestResponse *response)
{
	free(response->body);
	free(response);
}

/**
 * Función que escribe los datos de la respuesta del servicio web en la
 * estructura que corresponde a la respuesta del servicio
 * @author Esteban De La Fuente Rubio, DeLaF (esteban[at]sasco.cl)
 * @version 2017-02-22
 */
size_t rest_response_write(void *ptr, size_t size, size_t nmemb, struct RestResponse *response)
{
	size_t new_len = response->size + size*nmemb;
	response->body = realloc(response->body, new_len+1);
	if (response->body == NULL) {
		fprintf(stderr, "realloc() falló!\n");
		exit(EXIT_FAILURE);
	}
	memcpy(response->body+response->size, ptr, size*nmemb);
	response->body[new_len] = '\0';
	response->size = new_len;
	return size*nmemb;
}
