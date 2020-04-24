EXEC := smash

CC := gcc
SRCD := src
TSTD := test
BLDD := build
INCD := include

MAIN  := $(BLDD)/main.o

ALL_SRCF := $(shell find $(SRCD) -type f -name *.c)
ALL_OBJF := $(patsubst $(SRCD)/%,$(BLDD)/%,$(ALL_SRCF:.c=.o))
ALL_FUNCF := $(filter-out $(MAIN) $(AUX), $(ALL_OBJF))

TEST_SRC := $(shell find $(TSTD) -type f -name *.sh)

INC := -I $(INCD)

CFLAGS := -Wall -Werror -Wno-unused-variable -Wno-unused-function -MMD
DFLAGS := -g -DDEBUG

STD := -std=gnu11
# LIBS := -lcrypto
LIBS :=

CFLAGS += $(STD)

.PHONY: clean all debug tests

all: setup $(EXEC)

debug: CFLAGS += $(DFLAGS)
debug: all

setup: $(BLDD)

$(BLDD):
	mkdir -p $(BLDD)

$(EXEC): $(ALL_OBJF)
	$(CC) $^ -o $@ $(LIBS)

# TESTS=test/test_setup.sh test/test_decryption.sh test/test_encryption.sh test/test_fidelity.sh
# tests: $(EXEC) $(TESTS)
# 	sh test/test_setup.sh
# 	sh test/test_decryption.sh
# 	sh test/test_encryption.sh
# 	sh test/test_fidelity.sh

$(BLDD)/%.o: $(SRCD)/%.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -rf $(BLDD) $(EXEC)

.PRECIOUS: $(BLDD)/*.d
-include $(BLDD)/*.d