
all:
	b2

clean:
	rd /s /q bin

distclean: clean
	del M&BLanguageTemplateGenerator.exe

.PHONY: all clean distclean
