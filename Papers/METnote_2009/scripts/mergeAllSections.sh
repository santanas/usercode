#!/bin/sh

OUTFILE=CMS_AN_2010_029
TEXFILES="Title.tex Introduction.tex EventSelection.tex METStability.tex CaloNoise.tex DataVsMC_MB_900GeV.tex DataVsMC_MB_2360GeV.tex METDecomposition.tex Conclusions.tex Acknow.tex Bib.tex"
#TEXFILES="Title.tex Introduction.tex EventSelection.tex METStability.tex CaloNoise.tex DataVsMC_MB_900GeV.tex DataVsMC_MB_2360GeV.tex DataVsMC_DiJet_900GeV.tex DataVsMC_DiJet_2360GeV.tex Conclusions.tex Acknow.tex Bib.tex"

echo "Merging files: $TEXFILES"
echo "               into the output file ${OUTFILE}.tex"

cat > ${OUTFILE}.tex <<EOF
\documentclass{cmspaper}
\usepackage{lineno}
\usepackage{amsfonts,amsmath,amssymb}
\usepackage[dvips]{graphicx}
\usepackage{bm}
\usepackage{multirow}
\usepackage{subfigure}  % use for side-by-side figures
%\usepackage[a4paper]{hyperref}

\def\etmiss{\big\slash\hspace{-1.6ex}{E_\text{T}}}
\def\etmissB{\big\slash\hspace{-1.6ex}{\boldsymbol{E}_\text{T}}}
\def\exmiss{\big\slash\hspace{-1.6ex}{E_{x}}}
\def\eymiss{\big\slash\hspace{-1.6ex}{E_{y}}}
\def\sumet{\sum{E_\text{T}}}
\def\sumetB{\sum{\boldsymbol{E}_\text{T}}}

\begin{document}
\begin{linenumbers}
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
