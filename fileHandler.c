#include "fileHandler.h"

#include <stdio.h>
#include <stdlib.h>

#include "cmake-build-debug/_deps/cjson-src/cJSON.h"

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