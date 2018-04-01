# frozen_string_literal: true

require 'mkmf'

unless find_header('aerospike/aerospike.h')
  abort "Can't find the aerospike.h header"
end

abort "Can't find the Aerospike client library" unless have_library('aerospike')

$LDFLAGS << ' -lssl -lcrypto -lpthread -lm -lz'

create_makefile('aerospike_client')
