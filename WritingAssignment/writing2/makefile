TEX	= latex -shell-escape
BIBTEX	= bibtex
DVIPS	= dvips
DVIPDF  = dvipdft
XDVI	= xdvi -gamma 4
GH		= gv

all: writeup

writeup:
	latex writing2.tex
	bibtex writing2
	latex writing2.tex
	latex writing2.tex 
	dvips -R -Poutline -t letter writing2.dvi -o writing2.ps
	ps2pdf writing2.ps

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg 

.PHONY: all show clean ps pdf showps
