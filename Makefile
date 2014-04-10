# CS61C Sp14 Project 3
#
# ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
#

UNAME = $(shell uname)
HOSTNAME = $(shell hostname)

# Create a variable containing the CR
define newline


endef

# Warn if not running on hive
ifeq (,$(findstring hive,$(HOSTNAME)))
$(warning $(newline)$(newline)======================================================$(newline)WARNING: YOU ARE NOT RUNNING THIS CODE ON HIVE!$(newline)It is highly recommended that you use a hive computer.$(newline)======================================================$(newline))
endif

ifeq ("$(wildcard calc_dist.c)","")
$(error $(newline)$(newline)[ERROR] calc_dist.c does not exist. Make sure you create calc_dist.c first (see calc_dist.c.example for an example).$(newline)$(newline))
endif

CC = gcc -std=gnu99 -Wall -Werror
CFLAGS = -msse4 -fopenmp -O3 -pipe -fno-omit-frame-pointer
LDFLAGS = -fopenmp -lpthread

.PHONY: clean check

all: benchmark digit_rec

benchmark: benchmark.o calc_dist.o utils.o test_digitrec.o oracle.o
	$(CC) -o $@ $(LDFLAGS) $+

digit_rec: digit_rec.o calc_dist.o utils.o test_digitrec.o
	$(CC) -o $@ $(LDFLAGS) $+

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

check: digit_rec
	@echo "\n\nRunning BASIC tests... (no output means pass)"
	@echo "Checking basic_01.bmp"
	@./digit_rec basic_tests/basic_01.bmp | grep -q "I think your image is a 4" || echo "[FAIL] basic_01.bmp should be a 4"
	@./digit_rec basic_tests/basic_01.bmp | grep -q "4: 0.000" || echo "[FAIL] basic_01.bmp should have distance 0.000 from a 4"
	@echo "Checking basic_02.bmp"
	@./digit_rec basic_tests/basic_02.bmp | grep -q "I think your image is a 4" || echo "[FAIL] basic_02.bmp should be a 4"
	@echo "Checking flipped_01.bmp"
	@./digit_rec basic_tests/flipped_01.bmp | grep -q "I think your image is a 7" || echo "[FAIL] flipped_01.bmp should be a 7"
	@echo "Checking flipped_02.bmp"
	@./digit_rec basic_tests/flipped_02.bmp | grep -q "I think your image is a 3" || echo "[FAIL] flipped_02.bmp should be a 3"
	@echo "Checking flipped_x_3.bmp"
	@./digit_rec basic_tests/flipped_x_3.bmp | grep -q "I think your image is a 3" || echo "[FAIL] flipped_x_3.bmp should be a 3"
	@./digit_rec basic_tests/flipped_x_3.bmp | grep -q "3: 0.000" || echo "[FAIL] flipped_x_3.bmp should have distance 0.000 from a 3"
	@echo "Checking flipped_y_4.bmp"
	@./digit_rec basic_tests/flipped_y_4.bmp | grep -q "I think your image is a 4" || echo "[FAIL] flipped_y_4.bmp should be a 4"
	@./digit_rec basic_tests/flipped_y_4.bmp | grep -q "4: 0.000" || echo "[FAIL] flipped_y_4.bmp should have distance 0.000 from a 4"
	@echo "Checking other_02.bmp"
	@./digit_rec basic_tests/other_02.bmp | grep -q "I think your image is a 8" || echo "[FAIL] other_02.bmp should be a 8"
	@echo "Checking rotated_01.bmp"
	@./digit_rec basic_tests/rotated_01.bmp | grep -q "I think your image is a 5" || echo "[FAIL] rotated_01.bmp should be a 5"
	@echo "Checking rotated_2.bmp"
	@./digit_rec basic_tests/rotated_2.bmp | grep -q "I think your image is a 2" || echo "[FAIL] rotated_2.bmp should be a 2"
	@./digit_rec basic_tests/rotated_2.bmp | grep -q "2: 0.000" || echo "[FAIL] rotated_2.bmp should have distance 0.000 from a 2"
	@echo "Checking rotated_02.bmp"
	@./digit_rec basic_tests/rotated_02.bmp | grep -q "I think your image is a 2" || echo "[FAIL] rotated_02.bmp should be a 2"
	@echo "Checking rotated_5.bmp"
	@./digit_rec basic_tests/rotated_5.bmp | grep -q "I think your image is a 5" || echo "[FAIL] rotated_5.bmp should be a 5"
	@./digit_rec basic_tests/rotated_5.bmp | grep -q "5: 0.000" || echo "[FAIL] rotated_5.bmp should have distance 0.000 from a 5"
	@echo "Checking rotated_7.bmp"
	@./digit_rec basic_tests/rotated_7.bmp | grep -q "I think your image is a 7" || echo "[FAIL] rotated_7.bmp should be a 7"
	@./digit_rec basic_tests/rotated_7.bmp | grep -q "7: 0.000" || echo "[FAIL] rotated_7.bmp should have distance 0.000 from a 7"
	@echo "Checking translated.bmp"
	@./digit_rec basic_tests/translated.bmp | grep -q "I think your image is a 1" || echo "[FAIL] translated.bmp should be a 1"
	@echo "Checking translated_5_updated.bmp"
	@./digit_rec basic_tests/translated_5_updated.bmp | grep -q "I think your image is a 5" || echo "[FAIL] translated_5_updated.bmp should be a 5"

clean:
	rm -f benchmark.o digit_rec.o calc_dist.o utils.o test_digitrec.o benchmark digit_rec
