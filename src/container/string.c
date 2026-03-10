/**
 * @file string.c
 * @brief string 实现
 */

#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CXC_STRING_DEFAULT_CAPACITY 16
#define CXC_STRING_GROWTH_FACTOR 2

static cxc_bool_t cxc_string_reserve(cxc_string_t* str, cxc_size_t capacity) {
    if (str == NULL) {
        return false;
    }
    
    if (capacity <= str->capacity) {
        return true;
    }
    
    char* new_data = (char*)cxc_realloc(str->data, capacity);
    if (new_data == NULL) {
        return false;
    }
    
    str->data = new_data;
    str->capacity = capacity;
    return true;
}

cxc_string_t* cxc_string_create(cxc_size_t initial_capacity) {
    cxc_string_t* str = (cxc_string_t*)cxc_malloc(sizeof(cxc_string_t));
    if (str == NULL) {
        return NULL;
    }
    
    str->length = 0;
    str->capacity = initial_capacity > 0 ? initial_capacity : CXC_STRING_DEFAULT_CAPACITY;
    str->data = (char*)cxc_malloc(str->capacity);
    
    if (str->data == NULL) {
        cxc_free(str);
        return NULL;
    }
    
    str->data[0] = '\0';
    return str;
}

cxc_string_t* cxc_string_from_cstr(const char* cstr) {
    if (cstr == NULL) {
        return NULL;
    }
    
    cxc_size_t len = strlen(cstr);
    cxc_string_t* str = cxc_string_create(len + 1);
    if (str == NULL) {
        return NULL;
    }
    
    cxc_memcpy(str->data, cstr, len + 1);
    str->length = len;
    
    return str;
}

void cxc_string_destroy(cxc_string_t* str) {
    if (str == NULL) {
        return;
    }
    
    cxc_free(str->data);
    cxc_free(str);
}

void cxc_string_clear(cxc_string_t* str) {
    if (str != NULL) {
        str->length = 0;
        if (str->data != NULL) {
            str->data[0] = '\0';
        }
    }
}

cxc_bool_t cxc_string_append_char(cxc_string_t* str, char c) {
    if (str == NULL) {
        return false;
    }
    
    if (str->length + 1 >= str->capacity) {
        cxc_size_t new_capacity = str->capacity * CXC_STRING_GROWTH_FACTOR;
        if (!cxc_string_reserve(str, new_capacity)) {
            return false;
        }
    }
    
    str->data[str->length] = c;
    str->length++;
    str->data[str->length] = '\0';
    
    return true;
}

cxc_bool_t cxc_string_append_cstr(cxc_string_t* str, const char* cstr) {
    if (str == NULL || cstr == NULL) {
        return false;
    }
    
    cxc_size_t len = strlen(cstr);
    if (str->length + len + 1 >= str->capacity) {
        cxc_size_t new_capacity = str->capacity;
        while (new_capacity < str->length + len + 1) {
            new_capacity *= CXC_STRING_GROWTH_FACTOR;
        }
        if (!cxc_string_reserve(str, new_capacity)) {
            return false;
        }
    }
    
    cxc_memcpy(str->data + str->length, cstr, len + 1);
    str->length += len;
    
    return true;
}

cxc_bool_t cxc_string_append_string(cxc_string_t* str, const cxc_string_t* other) {
    if (str == NULL || other == NULL) {
        return false;
    }
    
    return cxc_string_append_cstr(str, other->data);
}

cxc_bool_t cxc_string_assign(cxc_string_t* str, const char* cstr) {
    if (str == NULL || cstr == NULL) {
        return false;
    }
    
    cxc_string_clear(str);
    return cxc_string_append_cstr(str, cstr);
}

int cxc_string_compare(const cxc_string_t* str1, const cxc_string_t* str2) {
    if (str1 == NULL && str2 == NULL) {
        return 0;
    }
    if (str1 == NULL) {
        return -1;
    }
    if (str2 == NULL) {
        return 1;
    }
    
    return strcmp(str1->data, str2->data);
}

cxc_size_t cxc_string_find(const cxc_string_t* str, const char* substr) {
    if (str == NULL || substr == NULL) {
        return (cxc_size_t)-1;
    }
    
    char* pos = strstr(str->data, substr);
    if (pos == NULL) {
        return (cxc_size_t)-1;
    }
    
    return (cxc_size_t)(pos - str->data);
}

cxc_string_t* cxc_string_substr(const cxc_string_t* str, cxc_size_t start, cxc_size_t length) {
    if (str == NULL || start >= str->length) {
        return NULL;
    }
    
    if (start + length > str->length) {
        length = str->length - start;
    }
    
    cxc_string_t* result = cxc_string_create(length + 1);
    if (result == NULL) {
        return NULL;
    }
    
    cxc_memcpy(result->data, str->data + start, length);
    result->data[length] = '\0';
    result->length = length;
    
    return result;
}

#ifdef __cplusplus
}
#endif
