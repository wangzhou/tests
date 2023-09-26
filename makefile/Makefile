_all=a b c d
__all=$(patsubst %, %_lib, $(_all))

all: $(_all) $(__all)

define test_rule=
$(1): test.o
	gcc test.o -o $(1)
$(1)_lib: test.o lib.o
	gcc test.o lib.o -DLIB -o $(1)_lib
endef

$(foreach n,$(_all),$(eval $(call test_rule,$(n))))

.PHONY: clean
clean:
	rm $(_all) $(__all) *.o
