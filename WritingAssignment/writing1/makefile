TEX	= latex -shell-escape
BIBTEX	= bibtex
DVIPS	= dvips
DVIPDF  = dvipdft
XDVI	= xdvi -gamma 4
GH		= gv

all: writeup

writeup:
	latex writing1.tex
	bibtex writing1
	latex writing1.tex
	latex writing1.tex 
	dvips -R -Poutline -t letter writing1.dvi -o writing1.ps
	ps2pdf writing1.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg 

.PHONY: all show clean ps pdf showps
