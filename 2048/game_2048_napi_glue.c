#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include "game_2048.h"

#define LOG_LEVEL_ERR  0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DBG  3

static int log_level = LOG_LEVEL_ERR;

#define log_level_set(ll) (log_level = ll)

#define log_err(fmt, ...) \
	do { if(log_level >= LOG_LEVEL_ERR) printf(fmt, __VA_ARGS__); } while(0)

#define log_warn(fmt, ...) \
	do { if(log_level >= LOG_LEVEL_WARN) printf(fmt, __VA_ARGS__); } while(0)

#define log_info(fmt, ...) \
	do { if(log_level >= LOG_LEVEL_INFO) printf(fmt, __VA_ARGS__); } while(0)

#define log_dbg(fmt, ...) \
	do { if(log_level >= LOG_LEVEL_DBG) printf(fmt, __VA_ARGS__); } while(0)

static void dbg_ptr(int** mat)
{
  log_dbg("mat = %p\n", mat);
  if (mat != NULL)
  {
    size_t z = 0;
    for(z = 0; z < 4; z++)
    {
      log_dbg("*(mat + %ld) = %p\n", z, *(mat + z));
    }
  }
}

static void* retrieve_external_ptr(napi_env env, napi_callback_info info)
{
  napi_status status = napi_ok;
  napi_value args[1];
  size_t argc = 1;
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);
  if (argc < 1)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }
  napi_valuetype valuetype0 = napi_undefined;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);
  log_info("valuetype0 = %d\n", valuetype0);
  if (valuetype0 != napi_external)
  {
    napi_throw_type_error(env, NULL, "Wrong arguments");
    return NULL;
  }
  void* dat = NULL;
  status = napi_get_value_external(env, args[0], &dat);
  assert(status == napi_ok);
  return dat;
}

static void wrapped_finalize(napi_env env, void* finalize_data, void* finalize_hint)
{
  //TODO: is it necessary to release the allocated memory
}

static napi_value added_get(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = retrieve_external_ptr(env, info);
  napi_value oa;
  status = napi_create_array_with_length(env, 4, &oa);
  assert(status == napi_ok);
  size_t i = 0;
  for(i = 0; i < 4; i++)
  {
    napi_value iab;
    status = napi_create_external_arraybuffer(env, *(mat + i), 4 * sizeof(int), wrapped_finalize, NULL, &iab);
    assert(status == napi_ok);
    napi_value ia;
    status = napi_create_typedarray(env, napi_int32_array, 4, iab, 0, &ia);
    assert(status == napi_ok);
    status = napi_set_element(env, oa, i, ia);
    assert(status == napi_ok);
  }
  return oa;
}

static napi_value wrapped_get(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = retrieve_external_ptr(env, info);
  game_2048_status st = GAME_2048_STATUS_NOT_OVER;
  game_2048_get_status(mat, &st);
  napi_value result;
  status = napi_create_int32(env, st, &result);
  assert(status == napi_ok);
  return result;
}

static napi_value wrapped_init(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = NULL;
  game_2048_init(&mat);
  dbg_ptr(mat);
  napi_value result;
  status = napi_create_external(env, mat, wrapped_finalize, NULL, &result);
  assert(status == napi_ok);
  napi_valuetype valuetype0 = napi_undefined;
  status = napi_typeof(env, result, &valuetype0);
  assert(status == napi_ok);
  log_info("valuetype0 = %d\n", valuetype0);
  return result;
}

static napi_value wrapped_add(napi_env env, napi_callback_info info) {
  int** mat = retrieve_external_ptr(env, info);
  game_2048_add_new_2(mat);
  return napi_ok;
}

static napi_value wrapped_left(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = retrieve_external_ptr(env, info);
  int changed = 0;
  game_2048_move_left(&mat, &changed);
  if(changed != 0)
  {
    dbg_ptr(mat);
  }
  napi_value result;
  status = napi_create_external(env, mat, wrapped_finalize, NULL, &result);
  assert(status == napi_ok);
  napi_valuetype valuetype1 = napi_undefined;
  status = napi_typeof(env, result, &valuetype1);
  assert(status == napi_ok);
  log_info("valuetype1 = %d\n", valuetype1);
  return result;
}

static napi_value wrapped_right(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = retrieve_external_ptr(env, info);
  int changed = 0;
  game_2048_move_right(&mat, &changed);
  if(changed != 0)
  {
    dbg_ptr(mat);
  }
  napi_value result;
  status = napi_create_external(env, mat, wrapped_finalize, NULL, &result);
  assert(status == napi_ok);
  napi_valuetype valuetype1 = napi_undefined;
  status = napi_typeof(env, result, &valuetype1);
  assert(status == napi_ok);
  log_info("valuetype1 = %d\n", valuetype1);
  return result;
}

static napi_value wrapped_up(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = retrieve_external_ptr(env, info);
  int changed = 0;
  game_2048_move_up(&mat, &changed);
  if(changed != 0)
  {
    dbg_ptr(mat);
  }
  napi_value result;
  status = napi_create_external(env, mat, wrapped_finalize, NULL, &result);
  assert(status == napi_ok);
  napi_valuetype valuetype1 = napi_undefined;
  status = napi_typeof(env, result, &valuetype1);
  assert(status == napi_ok);
  log_info("valuetype1 = %d\n", valuetype1);
  return result;
}

static napi_value wrapped_down(napi_env env, napi_callback_info info) {
  napi_status status = napi_ok;
  int** mat = retrieve_external_ptr(env, info);
  int changed = 0;
  game_2048_move_down(&mat, &changed);
  if(changed != 0)
  {
    dbg_ptr(mat);
  }
  napi_value result;
  status = napi_create_external(env, mat, wrapped_finalize, NULL, &result);
  assert(status == napi_ok);
  napi_valuetype valuetype1 = napi_undefined;
  status = napi_typeof(env, result, &valuetype1);
  assert(status == napi_ok);
  log_info("valuetype1 = %d\n", valuetype1);
  return result;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status = napi_ok;
  napi_property_descriptor desc[] =
  {
    DECLARE_NAPI_METHOD("peek", added_get),
    DECLARE_NAPI_METHOD("get", wrapped_get),
    DECLARE_NAPI_METHOD("init", wrapped_init),
    DECLARE_NAPI_METHOD("add", wrapped_add),
    DECLARE_NAPI_METHOD("left", wrapped_left),
    DECLARE_NAPI_METHOD("right", wrapped_right),
    DECLARE_NAPI_METHOD("up", wrapped_up),
    DECLARE_NAPI_METHOD("down", wrapped_down),
  };
  status = napi_define_properties(env, exports, sizeof(desc)/sizeof(desc[0]), (const napi_property_descriptor*)&desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

