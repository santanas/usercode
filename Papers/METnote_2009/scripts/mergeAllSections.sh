#!/bin/sh

OUTFILE=CMS_AN_2010_001
TEXFILES="Title.tex Introduction.tex EventSelection.tex METStability.tex CaloNoise.tex DataVsMC_MB_900GeV.tex DataVsMC_MB_2360GeV.tex Conclusions.tex Acknow.tex Bib.tex"
#TEXFILES="Title.tex Introduction.tex EventSelection.tex METStability.tex CaloNoise.tex DataVsMC_MB_900GeV.tex DataVsMC_MB_2360GeV.tex DataVsMC_DiJet_900GeV.tex DataVsMC_DiJet_2360GeV.tex Conclusions.tex Acknow.tex Bib.tex"

echo "Merging files: $TEXFILES"
echo "               into the output file ${OUTFILE}.tex"

cat > ${OUTFILE}.tex <<EOF
\documentclass[colclass=cmspaper]{combine}
\usepackage{lineno}
\usepackage{amsfonts,amsmath,amssymb}
\usepackage[dvips]{graphicx}
\usepackage{bm}

\def\etmiss{\big\slash\hspace{-1.6ex}{E_{\mathrm T}}}
\def\exmiss{\big\slash\hspace{-1.6ex}{E_{\mathrm x}}}
\def\eymiss{\big\slash\hspace{-1.6ex}{E_{\mathrm y}}}

\begin{document}
\begin{linenumbers}
\pagestyle{combine}
EOF

cat $TEXFILES >> ${OUTFILE}.tex

cat >> ${OUTFILE}.tex <<EOF
\end{linenumbers}
\end{document}
EOF

latex  ${OUTFILE}.tex
bibtex ${OUTFILE}.aux
latex  ${OUTFILE}.tex
latex  ${OUTFILE}.tex
dvipdf ${OUTFILE}.dvi ${OUTFILE}.pdf
