# frozen_string_literal: true

require 'aerospike'
require 'benchmark/ips'
require_relative '../lib/aerospike_client'

Aerospike.logger.level = Logger::WARN
$cli = ::Aerospike::Client.new('192.168.50.4:3000')
$c = AerospikeClient.new('192.168.50.4', 3000)

key = ::Aerospike::Key.new('test', 'eg-set', 'my_key')
bin = ::Aerospike::Bin.new(
  'data', ::Aerospike::StringValue.new('written value')
)

$cli.put(key, bin)
$c.put('test', 'eg-set', 'my_key', 'some_value')

puts $cli.get(key)
puts $c.get('test', 'eg-set', 'my_key')

Benchmark.ips do |x|
  x.report('Ruby get') { $cli.get(key) }
  x.report('C get') { $c.get('test', 'eg-set', 'my_key') }
  x.compare!
end

Benchmark.ips do |x|
  x.report('Ruby put') { $cli.get(key) }
  x.report('C put') { $c.put('test', 'eg-set', 'my_key', 'some_value') }
  x.compare!
end
