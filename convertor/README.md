# use flatbuffers as mininn model serialization tool

## step1: build flatc

run:
```sh
cd third_party/flatbuffers/

cmake -B build
cmake --build build
```

## step2: use flatc to gen py and cpp interface files

run:
```sh
../third_party/flatbuffers/build/flatc --python --cpp mininn.fbs
```

## step3: run test

run:
```sh
python3 test.py
```

you will get a "mininn_test.gynn"