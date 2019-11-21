#include "../includes/object.h"

Object init_object(int id) {
    Object obj = {0};
    obj._id = id;

    return obj;
}