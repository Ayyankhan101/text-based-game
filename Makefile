# Makefile for compiling the LaTeX report
.PHONY: all clean report

# Default target
all: report

# Compile the LaTeX report
report:
	pdflatex -interaction=nonstopmode wolf_pack_survival_report.tex

# Clean auxiliary files
clean:
	rm -f *.aux *.log *.toc *.out *.fls *.fdb_latexmk *.synctex.gz

# Clean everything including PDF
distclean: clean
	rm -f wolf_pack_survival_report.pdf

# Create PDF version
pdf: clean report
	mv wolf_pack_survival_report.pdf . 2>/dev/null || true

# View the report
view:
	evince wolf_pack_survival_report.pdf &