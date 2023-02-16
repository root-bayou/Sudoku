EXE = sudoku
all: build

build:
	@cd src && $(MAKE)
	@cp -f src/$(EXE) ./
	
check: build
	@cd tests && $(MAKE)
  
clean:
	@cd src && $(MAKE) clean 
	@cd tests && $(MAKE) clean
	@rm -f $(EXE)
  
help:
  @echo "Usage:"

.PHONY: all build check clean help
