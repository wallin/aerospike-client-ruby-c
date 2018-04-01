#include <ruby.h>
#include <aerospike/aerospike.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>

VALUE rb_aerospike_client_initialize(VALUE self, VALUE rb_host_string, VALUE rb_host_port);
VALUE rb_aerospike_client_get(VALUE self, VALUE rb_namespace, VALUE rb_set, VALUE rb_key);
VALUE rb_aerospike_client_put(VALUE self, VALUE rb_namespace, VALUE rb_set, VALUE rb_key, VALUE rb_value);

VALUE AerospikeClient = Qnil;

VALUE rb_aerospike_client_initialize(VALUE self, VALUE rb_host_string, VALUE rb_host_port) {
  Check_Type(rb_host_string, T_STRING);
  aerospike* p_as;
  as_error err;

  Data_Get_Struct(self, aerospike, p_as);

  as_config_lua lua;
  as_config_lua_init(&lua);
  aerospike_init_lua(&lua);

  as_config config;
  as_config_init(&config);

  if (!as_config_add_hosts(&config, StringValuePtr(rb_host_string), NUM2UINT(rb_host_port))) {
    // as_event_close_loops();
    rb_raise(rb_eTypeError, "Invalid host(s)");
  }

  aerospike_init(p_as, &config);

  if (aerospike_connect(p_as, &err) != AEROSPIKE_OK) {
    // printf("aerospike_connect() returned %d - %s", err.code, err.message);
    // as_event_close_loops();
    // aerospike_destroy(p_as);
    rb_raise(rb_eTypeError, "aerospike_connect() failed");
  }

  return Qnil;
}

VALUE rb_aerospike_client_get(VALUE self, VALUE rb_namespace, VALUE rb_set, VALUE rb_key)
{
  as_error err;
  as_record* p_rec = NULL;
  as_key g_key;
  aerospike* as;
  Data_Get_Struct(self, aerospike, as);

  as_key_init_str(
    &g_key,
    StringValuePtr(rb_namespace),
    StringValuePtr(rb_set),
    StringValuePtr(rb_key)
  );

  if (aerospike_key_get(as, &err, NULL, &g_key, &p_rec) != AEROSPIKE_OK) {
    return Qnil;
  }

  VALUE r_hash = rb_hash_new();
  as_record_iterator it;
  as_record_iterator_init(&it, p_rec);

  as_bin* p_bin = NULL;
  while (as_record_iterator_has_next(&it)) {
    p_bin = as_record_iterator_next(&it);
    // TODO: set correct value type
    rb_hash_aset(
      r_hash,
      rb_str_new2(as_bin_get_name(p_bin)),
      rb_str_new2(as_val_tostring(as_bin_get_value(p_bin)))
    );
  }

  as_record_iterator_destroy(&it);
  as_record_destroy(p_rec);

  return r_hash;
}

VALUE rb_aerospike_client_put(VALUE self, VALUE rb_namespace, VALUE rb_set, VALUE rb_key, VALUE rb_value)
{
  as_error err;
  as_key g_key;
  aerospike* as;
  Data_Get_Struct(self, aerospike, as);

  as_key_init_str(
    &g_key,
    StringValuePtr(rb_namespace),
    StringValuePtr(rb_set),
    StringValuePtr(rb_key)
  );

  // TODO: handle multiple bins and different types
  as_record rec;
  as_record_inita(&rec, 1);
  as_record_set_str(&rec, "data", StringValuePtr(rb_value));

  if (aerospike_key_put(as, &err, NULL, &g_key, &rec) != AEROSPIKE_OK) {
    rb_raise(rb_eTypeError, "aerospike_key_put() failed");
  }
  return rb_value;
}

static void deallocate(aerospike * p_as)
{
  as_error err;
  aerospike_close(p_as, &err);
  aerospike_destroy(p_as);
  free(p_as);
}

static VALUE allocate(VALUE klass)
{
  aerospike* p_as = malloc(sizeof(aerospike));

  return Data_Wrap_Struct(klass, NULL, deallocate, p_as);
}

void Init_aerospike_client(void) {
  AerospikeClient = rb_define_class("AerospikeClient", rb_cObject);
  rb_define_alloc_func(AerospikeClient, allocate);

  rb_define_method(AerospikeClient, "initialize", rb_aerospike_client_initialize, 2);
  rb_define_method(AerospikeClient, "get", rb_aerospike_client_get, 3);
  rb_define_method(AerospikeClient, "put", rb_aerospike_client_put, 4);
}