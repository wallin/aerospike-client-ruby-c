# aerospike-client-ruby-c

POC Ruby extension that wraps the [aerospike C client](https://github.com/aerospike/aerospike-client-c)

Primary purpose is to benchmark simple operations against the [official Ruby client](https://github.com/aerospike/aerospike-client-ruby)

NOTE: this wrapper is by far not complete. Eg. get and put implementations only handles strings

## Install

Download and install the [aerospike C library](https://github.com/aerospike/aerospike-client-c)

## Run

```bash
$ bundle install
$ rake compile
$ ruby benchmark/put_get
```

## Benchmarks

```bash

Warming up --------------------------------------
            Ruby get   131.000  i/100ms
               C get   195.000  i/100ms
Calculating -------------------------------------
            Ruby get      1.473k (±27.5%) i/s -      6.943k in   5.108522s
               C get      2.325k (±21.5%) i/s -     11.310k in   5.062703s

Comparison:
               C get:     2324.7 i/s
            Ruby get:     1472.6 i/s - same-ish: difference falls within error

Warming up --------------------------------------
            Ruby put   159.000  i/100ms
               C put   229.000  i/100ms
Calculating -------------------------------------
            Ruby put      1.696k (±27.5%) i/s -      8.109k in   5.030362s
               C put      2.441k (±33.8%) i/s -     10.992k in   5.077228s

Comparison:
               C put:     2440.7 i/s
            Ruby put:     1696.4 i/s - same-ish: difference falls within error
```