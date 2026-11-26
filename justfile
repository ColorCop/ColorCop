# Build the portable test suite
build:
    cmake -B build
    cmake --build build

# Execute Catch2 tests
test: build
    ./build/ColorCopTests

# Run discovered tests via CTest
ctest: build
    ctest --test-dir build --output-on-failure

lint:
    pip install -r requirements.txt
    cpplint --recursive --exclude=.git --exclude=build --exclude=ColorCop/resource.h --exclude=ColorCop/version.h .
    cppcheck --project=ColorCop.sln --library=mfc.cfg --enable=warning,style,performance,portability --inline-suppr --error-exitcode=1 --check-level=exhaustive
clean:
    rm -rf build

rebuild: clean test
