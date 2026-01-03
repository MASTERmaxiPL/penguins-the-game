#include "fileHandler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../../cmake-build-debug/_deps/cjson-src/cJSON.h"

/**
 * Saves a cJSON object to a file in JSON format.
 *
 * @param json Pointer to the cJSON object to save.
 * @param path File path where the JSON should be saved.
 * @return true if the operation was successful, false otherwise.
 */
bool SaveJsonToFile(const cJSON *json, const char *path) {
    if (!json || !path) return false;
    char *str = cJSON_Print(json);
    if (!str) return false;

    FILE *fp = fopen(path, "w");
    if (!fp) {
        cJSON_free(str);
        return false;
    }

    if (fputs(str, fp) == EOF) {
        fclose(fp);
        cJSON_free(str);
        return false;
    }

    fclose(fp);
    cJSON_free(str);
    return true;
}

/**
 * Loads a cJSON object from a file containing JSON data.
 *
 * @param path File path from which to load the JSON.
 * @return Pointer to the loaded cJSON object, or NULL on failure.
 */
cJSON *LoadJsonFromFile(const char *path) {
    if (!path) return NULL;
    FILE *fp = fopen(path, "r");
    if (!fp) return NULL;

    if (fseek(fp, 0, SEEK_END) != 0) { fclose(fp); return NULL; }
    const long len = ftell(fp);
    if (len < 0) { fclose(fp); return NULL; }
    if (fseek(fp, 0, SEEK_SET) != 0) { fclose(fp); return NULL; }

    char *buf = malloc((size_t)len + 1);
    if (!buf) { fclose(fp); return NULL; }

    const size_t read = fread(buf, 1, (size_t)len, fp);
    fclose(fp);
    buf[read] = '\0';

    cJSON *json = cJSON_Parse(buf);
    free(buf);
    return json;
}