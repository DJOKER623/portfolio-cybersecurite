# Makefile pour compiler rapport2.tex

# Variables
TEX = pdflatex
BIBTEX = bibtex
TARGET = rapport2
SRC = $(TARGET).tex

# Fichiers générés
PDF = $(TARGET).pdf
AUX = $(TARGET).aux
LOG = $(TARGET).log
OUT = $(TARGET).out
TOC = $(TARGET).toc
BBL = $(TARGET).bbl
BLG = $(TARGET).blg

# Règle par défaut
all: $(PDF)

# Compilation du PDF
$(PDF): $(SRC)
	$(TEX) $(SRC)
	$(TEX) $(SRC)

# Compilation avec bibliographie
bib: $(SRC)
	$(TEX) $(SRC)
	$(BIBTEX) $(TARGET)
	$(TEX) $(SRC)
	$(TEX) $(SRC)

# Nettoyage des fichiers auxiliaires
clean:
	rm -f $(AUX) $(LOG) $(OUT) $(TOC) $(BBL) $(BLG)

# Nettoyage complet (incluant le PDF)
cleanall: clean
	rm -f $(PDF)

# Recompilation complète
rebuild: cleanall all

# Afficher le PDF
view: $(PDF)
	xdg-open $(PDF) 2>/dev/null || open $(PDF) 2>/dev/null || start $(PDF)

.PHONY: all bib clean cleanall rebuild view