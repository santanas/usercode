
% $Id: varenna.bst,v 1.1 2007/07/12 07:17:43 marco Exp $


%%
%% Filename   = "varenna.bst"
%% Source     = "/var/local/devel/macro/cimento/varenna.bst,v"
%% Authors    = "Marco Bellacosa and Philip G. Ratcliffe"
%% Abstract   = "BibTeX bibliography style `varenna.bst' for BibTeX
%%               versions 0.99a or later and LaTeX version 2e
%%               produces numeric-label bibliography items in
%%               a form typical for Varenna style"
%% 

%%
%% For bugs, comments, suggestions email to: marco@sif.it
%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ENTRY
  { address
    author
    booktitle
    chapter
    edition
    editor
    howpublished
    institution
    journal
    key
    month
    note
    number
    organization
    pages
    publisher
    school
    series
    title
    type
    url
    volume
    year
% new fields (PGR)
    eprint
    SLACcitation
  }
  {}
  { label }

INTEGERS { output.state before.all mid.sentence after.sentence after.block }

FUNCTION {init.state.consts}
{ #0 'before.all :=
  #1 'mid.sentence :=
  #2 'after.sentence :=
  #3 'after.block :=
}

STRINGS { s t }

FUNCTION {output.nonnull}
{ 's :=
  output.state mid.sentence =
    { ", " * write$ }
    { output.state after.block =
        { add.period$ write$
          newline$
          "\newblock " write$
        }
        { output.state before.all =
            'write$
            { add.period$ " " * write$ }
          if$
        }
      if$
      mid.sentence 'output.state :=
    }
  if$
  s
}

FUNCTION {output}
{ duplicate$ empty$
    'pop$
    'output.nonnull
  if$
}

FUNCTION {output.check}
{ 't :=
  duplicate$ empty$
    { pop$ "empty " t * " in " * cite$ * warning$ }
    'output.nonnull
  if$
}

FUNCTION {write.SLACcitation}
{ SLACcitation empty$
    { skip$ }
    { SLACcitation write$ newline$ }
  if$
}

FUNCTION {fin.entry}
{ add.period$
  write$
  newline$
  write.SLACcitation
}

FUNCTION {new.block}
{ output.state before.all =
    'skip$
    { after.block 'output.state := }
  if$
}

FUNCTION {new.sentence}
{ output.state after.block =
    'skip$
    { output.state before.all =
        'skip$
        { after.sentence 'output.state := }
      if$
    }
  if$
}

FUNCTION {not}
{   { #0 }
    { #1 }
  if$
}

FUNCTION {and}
{   'skip$
    { pop$ #0 }
  if$
}

FUNCTION {or}
{   { pop$ #1 }
    'skip$
  if$
}

FUNCTION {new.block.checka}
{ empty$
    'skip$
    'new.block
  if$
}

FUNCTION {new.block.checkb}
{ empty$
  swap$ empty$
  and
    'skip$
    'new.block
  if$
}

FUNCTION {new.sentence.checka}
{ empty$
    'skip$
    'new.sentence
  if$
}

FUNCTION {new.sentence.checkb}
{ empty$
  swap$ empty$
  and
    'skip$
    'new.sentence
  if$
}

FUNCTION {field.or.null}
{ duplicate$ empty$
    { pop$ "" }
    'skip$
  if$
}

FUNCTION {emphasise}
{ duplicate$ empty$
    { pop$ "" }
    { "\emph{" swap$ * "}" * }
  if$
}

FUNCTION {texttt}
{ duplicate$ empty$
    { pop$ "" }
    { "\texttt{" swap$ * "}" * }
  if$
}

FUNCTION {parenthesise}
{ duplicate$ empty$
    { pop$ "" }
    { "(" swap$ * ")" * }
  if$
}

FUNCTION {VarennaNAME}
{ duplicate$ empty$
    { pop$ "" }
    { "\NAME{" swap$ * "}" * }
  if$
}

FUNCTION {VarennaTITLE}
{ duplicate$ empty$
    { pop$ "" }
    { "\TITLE{" swap$ * "}" * }
  if$
}

FUNCTION {capitalize}
{ "u" change.case$ "t" change.case$ }

FUNCTION {space.word}
{ " " swap$ * " " * }

FUNCTION {Varenna.and}
{ "\atque"}

FUNCTION {Varenna.etal}
{ "\etal" }

FUNCTION {Varenna.editors}
{ "Eds." }

FUNCTION {Varenna.editor}
{ "Ed." }

FUNCTION {Varenna.edby}
{ "edited by" }

FUNCTION {Varenna.edition}
{ "Edition" }

FUNCTION {Varenna.volume}
{ "Vol." }

FUNCTION {Varenna.of}
{ "of" }

FUNCTION {Varenna.number}
{ "no." }

FUNCTION {Varenna.nr}
{ "no." }

FUNCTION {Varenna.in}
{ "in" }

FUNCTION {Varenna.pages}
{ "pp." }

FUNCTION {Varenna.page}
{ "p." }

FUNCTION {Varenna.chapter}
{ "Ch." }

FUNCTION {Varenna.techrep}
{ "Tech. Rep." }

FUNCTION {Varenna.mthesis}
{ "Master's thesis" }

FUNCTION {Varenna.phdthesis}
{ "Ph.D. thesis" }

FUNCTION {Varenna.first}
{ "1st" }

FUNCTION {Varenna.second}
{ "2nd" }

FUNCTION {Varenna.third}
{ "3rd" }

FUNCTION {Varenna.fourth}
{ "4th" }

FUNCTION {Varenna.fifth}
{ "5th" }

FUNCTION {Varenna.st}
{ "st" }

FUNCTION {Varenna.nd}
{ "nd" }

FUNCTION {Varenna.rd}
{ "rd" }

FUNCTION {Varenna.th}
{ "th" }

MACRO {jan} {"Jan."}

MACRO {feb} {"Feb."}

MACRO {mar} {"Mar."}

MACRO {apr} {"Apr."}

MACRO {may} {"May"}

MACRO {jun} {"Jun."}

MACRO {jul} {"Jul."}

MACRO {aug} {"Aug."}

MACRO {sep} {"Sep."}

MACRO {oct} {"Oct."}

MACRO {nov} {"Nov."}

MACRO {dec} {"Dec."}

FUNCTION {eng.ord}
{ duplicate$ "1" swap$ *
  #-2 #1 substring$ "1" =
     { Varenna.th * }
     { duplicate$ #-1 #1 substring$
       duplicate$ "1" =
         { pop$ Varenna.st * }
         { duplicate$ "2" =
             { pop$ Varenna.nd * }
             { "3" =
                 { Varenna.rd * }
                 { Varenna.th * }
               if$
             }
           if$
          }
       if$
     }
   if$
}

FUNCTION {write.url}
{ url empty$
    { skip$ }
    { "\newline\urlprefix\url{" url * "}" * write$ newline$ }
  if$
}

INTEGERS { nameptr namesleft numnames }

FUNCTION {format.names}
{ 's :=
  #1 'nameptr :=
  s num.names$ 'numnames :=
  numnames 'namesleft :=
    { namesleft #0 > }
    { s nameptr
      "{vv~}{ll}{, jj}{ f.}" format.name$ % initials separate
%      "{vv~}{ll}{, jj}{~f{}}" format.name$ % initials together
      't :=
      nameptr #1 >
        {
          namesleft #1 >
            { ", " * t * }
            {
              numnames #1 >
                { "" * }
                'skip$
              if$
              t "others" =
                { " " * Varenna.etal * }
                { Varenna.and space.word * t * }
              if$
            }
          if$
        }
        't
      if$
      nameptr #1 + 'nameptr :=
      namesleft #1 - 'namesleft :=
    }
  while$
}

FUNCTION {format.names.ed}
{ format.names }

FUNCTION {format.authors}
{ author empty$
    { "" }
    { author format.names VarennaNAME }
  if$
}

FUNCTION {format.editors}
{ editor empty$
    { "" }
    { editor format.names VarennaNAME
      editor num.names$ #1 >
        { " " * Varenna.editors parenthesise * }
        { " " * Varenna.editor parenthesise * }
      if$
    }
  if$
}

FUNCTION {format.in.editors}
{ editor empty$
    { "" }
    { "edited by " editor format.names.ed VarennaNAME * }
  if$
}

FUNCTION {format.note}
{
 note empty$
    { "" }
    { note #1 #1 substring$
      duplicate$ "{" =
        'skip$
        { output.state mid.sentence =
          { "l" }
          { "u" }
        if$
        change.case$
        }
      if$
      note #2 global.max$ substring$ *
    }
  if$
}

FUNCTION {format.title}
{ title empty$
    { "" }
    { title "t" change.case$ VarennaTITLE }
  if$
}

FUNCTION {output.bibitem}
{ newline$
  "\bibitem{" write$
  cite$ write$
  "}" write$
  newline$
  ""
  before.all 'output.state :=
}

FUNCTION {n.dashify}
{
  't :=
  ""
    { t empty$ not }
    { t #1 #1 substring$ "-" =
        { t #1 #2 substring$ "--" = not
            { "--" *
              t #2 global.max$ substring$ 't :=
            }
            {   { t #1 #1 substring$ "-" = }
                { "-" *
                  t #2 global.max$ substring$ 't :=
                }
              while$
            }
          if$
        }
        { t #1 #1 substring$ *
          t #2 global.max$ substring$ 't :=
        }
      if$
    }
  while$
}

FUNCTION {n.first.item}
{ 't :=
  ""
    { t empty$ not t #1 #1 substring$ "-" = not and }
    { t #1 #1 substring$ *
      t #2 global.max$ substring$ 't :=
    }
  while$
}

FUNCTION {format.date}
{ year empty$
    { month empty$
        { "" }
        { "there's a month but no year in " cite$ * warning$
          month
        }
      if$
    }
    { month empty$
        'year
        { month " " * year * }
      if$
    }
  if$
  duplicate$ empty$
    'skip$
    { before.all 'output.state :=
      parenthesise
      " " swap$ *
    }
  if$
}

FUNCTION{format.year}
{ year duplicate$ empty$
    { "empty year in " cite$ * warning$ pop$ "" }
    'skip$
  if$
}

FUNCTION {format.btitle}
{ title VarennaTITLE }

FUNCTION {tie.or.space.connect}
{ duplicate$ text.length$ #3 <
    { "~" }
    { " " }
  if$
  swap$ * *
}

FUNCTION {either.or.check}
{ empty$
    'pop$
    { "can't use both " swap$ * " fields in " * cite$ * warning$ }
  if$
}

FUNCTION {format.bvolume}
{ volume empty$
    { "" }
    { Varenna.volume volume tie.or.space.connect
      series empty$
        'skip$
        { Varenna.of space.word * series emphasise * }
      if$
      "volume and number" number either.or.check
    }
  if$
}

FUNCTION {format.number.series}
{ volume empty$
    { number empty$
        { series field.or.null }
        { output.state mid.sentence =
            { Varenna.number }
            { Varenna.number capitalize }
          if$
          number tie.or.space.connect
          series empty$
            { "there's a number but no series in " cite$ * warning$ }
            { Varenna.in space.word * series emphasise * }
          if$
        }
      if$
    }
    { "" }
  if$
}

FUNCTION {is.num}
{ chr.to.int$
  duplicate$ "0" chr.to.int$ < not
  swap$ "9" chr.to.int$ > not and
}

FUNCTION {extract.num}
{ duplicate$ 't :=
  "" 's :=
  { t empty$ not }
  { t #1 #1 substring$
    t #2 global.max$ substring$ 't :=
    duplicate$ is.num
      { s swap$ * 's := }
      { pop$ "" 't := }
    if$
  }
  while$
  s empty$
    'skip$
    { pop$ s }
  if$
}

FUNCTION {convert.edition}
{ edition extract.num "l" change.case$ 's :=
  s "first" = s "1" = or
    { Varenna.first 't := }
    { s "second" = s "2" = or
        { Varenna.second 't := }
        { s "third" = s "3" = or
            { Varenna.third 't := }
            { s "fourth" = s "4" = or
                { Varenna.fourth 't := }
                { s "fifth" = s "5" = or
                    { Varenna.fifth 't := }
                    { s #1 #1 substring$ is.num
                        { s eng.ord 't := }
                        { edition 't := }
                      if$
                    }
                  if$
                }
              if$
            }
          if$
        }
      if$
    }
  if$
  t
}

FUNCTION {format.edition}
{ edition empty$
    { "" }
    { output.state mid.sentence =
        { convert.edition "l" change.case$ " " * Varenna.edition * }
        { convert.edition "t" change.case$ " " * Varenna.edition * }
      if$
    }
  if$
}

INTEGERS { multiresult }

FUNCTION {multi.page.check}
{ 't :=
  #0 'multiresult :=
    { multiresult not
      t empty$ not
      and
    }
    { t #1 #1 substring$
      duplicate$ "-" =
      swap$ duplicate$ "," =
      swap$ "+" =
      or or
        { #1 'multiresult := }
        { t #2 global.max$ substring$ 't := }
      if$
    }
  while$
  multiresult
}

FUNCTION {format.art.pages}
{ pages empty$
    { "" }
    { pages n.first.item }
  if$
}

FUNCTION {format.pages}
{ pages empty$
    { "" }
    { pages multi.page.check
        { Varenna.pages pages n.dashify tie.or.space.connect }
        { Varenna.page pages tie.or.space.connect }
      if$
    }
  if$
}

INTEGERS {z}
STRINGS {a}

FUNCTION {format.jour.vol.year.pages}
{ "\IN{"
  journal empty$
    { "" * "there's no journal in " cite$ * warning$ }
    { journal * "" *}
  if$
  volume empty$
    { "}{" * }
    { volume #1 #1 substring$ duplicate$ 'a := chr.to.int$ 'z :=
      z #57 >
        { a * "}{" * volume #2 global.max$ substring$ *
        }
        { "}{" * volume * }
      if$
    }
  if$
  "}{" * format.year * "}{" * format.art.pages * "}" *
}

FUNCTION {format.chapter.pages}
{ chapter empty$
    { "" }
    { type empty$
        { Varenna.chapter }
        { type "l" change.case$ }
      if$
      chapter tie.or.space.connect
    }
  if$
}

FUNCTION {format.in.ed.booktitle}
{ booktitle empty$
    { "" }
    { pages empty$
        { type$ "inproceedings" =
            { "presented at " }
            { Varenna.in " " * }
          if$
        }
        { Varenna.in " " *
          type$ "inproceedings" =
            { "proc. of " *}
            'skip$
          if$
        }
      if$
      booktitle VarennaTITLE *
      editor empty$
        'skip$
        { ", " * format.in.editors * }
      if$
    }
  if$
}

FUNCTION {empty.misc.check}
{ author empty$ title empty$ howpublished empty$
  month empty$ year empty$ note empty$
  and and and and and
    { "all relevant fields are empty in " cite$ * warning$ }
    'skip$
  if$
}

FUNCTION {format.thesis.type}
{ type empty$
    'skip$
    { pop$
      type "t" change.case$
    }
  if$
}

FUNCTION {format.tr.number}
{ type empty$
    { Varenna.techrep }
    'type
  if$
  institution empty$ number empty$ and
    { "t" change.case$ }
    'skip$
  if$
  institution empty$
    'skip$
    { institution tie.or.space.connect }
  if$
  number empty$
    'skip$
    { number tie.or.space.connect }
  if$
}

FUNCTION {format.eprint}
{ eprint empty$
    { "" }
    { eprint texttt }
  if$
}

FUNCTION {format.article.crossref}
{ key empty$
    { journal empty$
        { "need key or journal for " cite$ * " to crossref " * crossref *
          warning$
          ""
        }
        { Varenna.in " " * journal emphasise * }
      if$
    }
    { Varenna.in " " * key * " " *}
  if$
  " \cite{" * crossref * "}" *
}

FUNCTION {format.crossref.editor}
{ editor #1 "{vv~}{ll}" format.name$
  editor num.names$ duplicate$
  #2 >
    { pop$
      " " * Varenna.etal *
    }
    { #2 <
        'skip$
        { editor #2 "{ff }{vv }{ll}{ jj}" format.name$ "others" =
            {
              " " * Varenna.etal *
            }
            { Varenna.and space.word * editor #2 "{vv~}{ll}" format.name$
              * }
          if$
        }
      if$
    }
  if$
}

FUNCTION {format.book.crossref}
{ volume empty$
    { "empty volume in " cite$ * "'s crossref of " * crossref * warning$
      Varenna.in " " *
    }
    { Varenna.volume volume tie.or.space.connect
      Varenna.of space.word *
    }
  if$
  editor empty$
  editor field.or.null author field.or.null =
  or
    { key empty$
        { series empty$
            { "need editor, key, or series for " cite$ * " to crossref " *
              crossref * warning$
              "" *
            }
            { series emphasise * }
          if$
        }
        { key * }
      if$
    }
    { format.crossref.editor * }
  if$
  " \cite{" * crossref * "}" *
}

FUNCTION {format.incoll.inproc.crossref}
{
  editor empty$
  editor field.or.null author field.or.null =
  or
    { key empty$
        { booktitle empty$
            { "need editor, key, or booktitle for " cite$ * " to crossref " *
              crossref * warning$
              ""
            }
            { Varenna.in " " * booktitle * }
          if$
        }
        { Varenna.in " " * key * " " *}
      if$
    }
    { Varenna.in " " * format.crossref.editor * " " *}
  if$
  " \cite{" * crossref * "}" *
}

FUNCTION {format.org.or.pub}
{ 't :=
  ""
  year empty$
    { "empty year in " cite$ * warning$ }
    'skip$
  if$
  address empty$ t empty$ and year empty$ and
    'skip$
    { t empty$
        { address empty$
          'skip$
          { address * }
          if$
        }
        { t *
          address empty$
            'skip$
            { ", " * address * }
          if$
        }
      if$
      parenthesise
      " " swap$ *
      before.all 'output.state :=
      year empty$
        'skip$
        { t empty$ address empty$ and
            'skip$
            { " " * }
          if$
          year *
        }
      if$
    }
  if$
}

FUNCTION {format.publisher.address}
{ publisher empty$
    { "empty publisher in " cite$ * warning$
      ""
    }
    { publisher }
  if$
  format.org.or.pub
}

FUNCTION {format.organization.address}
{ organization empty$
    { "" }
    { organization }
  if$
  format.org.or.pub
}

FUNCTION {article}
{ output.bibitem
  format.authors "author" output.check
  crossref missing$
    { format.jour.vol.year.pages output }
    { format.article.crossref output.nonnull
      format.pages output
    }
  if$
  format.note output
  fin.entry
  write.url
}

FUNCTION {book}
{ output.bibitem
  author empty$
    { format.editors "author and editor" output.check }
    { format.authors output.nonnull
      crossref missing$
        { "author and editor" editor either.or.check }
        'skip$
      if$
    }
  if$
  format.btitle "title" output.check
  crossref missing$
    { format.edition output
      format.bvolume output
      format.number.series output
      format.publisher.address output
    }
    { format.book.crossref output.nonnull }
  if$
  format.note output
  fin.entry
  write.url
}

FUNCTION {booklet}
{ output.bibitem
  format.authors output
  format.title "title" output.check
  howpublished output
  address output
  format.note output
  format.date output
  fin.entry
  write.url
}

FUNCTION {inbook}
{ output.bibitem
  author empty$
    { format.editors "author and editor" output.check
    }
    { format.authors output.nonnull
      crossref missing$
        { "author and editor" editor either.or.check }
        'skip$
      if$
    }
  if$
  format.btitle "title" output.check
  crossref missing$
    {
      format.edition output
      format.bvolume output
      format.number.series output
      format.publisher.address output
      format.chapter.pages "chapter and pages" output.check
    }
    {
      format.chapter.pages "chapter and pages" output.check
      format.book.crossref output.nonnull
    }
  if$
  format.pages "pages" output.check
  format.note output
  fin.entry
  write.url
}

FUNCTION {incollection}
{ output.bibitem
  format.authors "author" output.check
  format.title "title" output.check
  crossref missing$
    { format.in.ed.booktitle "booktitle" output.check
      format.edition output
      format.bvolume output
      format.number.series output
      format.publisher.address output
      format.chapter.pages output
    }
    { format.incoll.inproc.crossref output.nonnull
      format.chapter.pages output
    }
  if$
  format.pages "pages" output.check
  format.note output
  fin.entry
  write.url
}

FUNCTION {inproceedings}
{ output.bibitem
  format.authors "author" output.check
  format.title "title" output.check
  crossref missing$
    { format.in.ed.booktitle "booktitle" output.check
      format.edition output
      format.bvolume output
      format.number.series output
      publisher empty$
        { format.organization.address output }
        { organization output
          format.publisher.address output
        }
      if$
    }
    { format.incoll.inproc.crossref output.nonnull }
  if$
  format.pages "pages" output.check
  format.note output
  fin.entry
  write.url
}

FUNCTION {conference} { inproceedings }

FUNCTION {manual}
{ output.bibitem
  author empty$
    { organization empty$
        'skip$
        { organization output.nonnull
          address output
        }
      if$
    }
    { format.authors output.nonnull }
  if$
  format.btitle "title" output.check
  author empty$
    { organization empty$
    {
          address output
        }
        'skip$
      if$
    }
    {
      organization output
      address output
    }
  if$
  format.edition output
  format.note output
  format.date output
  fin.entry
  write.url
}

FUNCTION {mastersthesis}
{ output.bibitem
  format.authors "author" output.check
  format.title "title" output.check
  Varenna.mthesis format.thesis.type output.nonnull
  school "school" output.check
  address output
  format.note output
  format.date "year" output.check
  fin.entry
  write.url
}

FUNCTION {misc}
{ output.bibitem
  format.authors output
  format.title output
  howpublished output
  format.note output
  format.date output
  fin.entry
  write.url
  empty.misc.check
}

FUNCTION {phdthesis}
{ output.bibitem
  format.authors "author" output.check
  format.title "title" output.check
  Varenna.phdthesis format.thesis.type output.nonnull
  school "school" output.check
  address output
  format.note output
  format.date "year" output.check
  fin.entry
  write.url
}

FUNCTION {proceedings}
{ output.bibitem
  editor empty$
    { organization output }
    { format.editors output.nonnull }
  if$
  format.btitle "title" output.check
  format.bvolume output
  format.number.series output
  editor empty$
    { publisher empty$
        'skip$
        {
          format.publisher.address output
        }
      if$
    }
    { publisher empty$
        {
          format.organization.address output }
        {
          organization output
          format.publisher.address output
        }
      if$
     }
  if$
  format.note output
%  format.date "year" output.check
  fin.entry
  write.url
}

FUNCTION {techreport}
{ output.bibitem
  format.authors "author" output.check
  format.tr.number output.nonnull
  institution "institution" output.check
  address output
  format.note output
  format.date "year" output.check
  format.eprint output
  fin.entry
  write.url
}

FUNCTION {unpublished}
{ output.bibitem
  format.authors "author" output.check
  format.title "title" output.check
  format.note "note" output.check
  format.date output
  fin.entry
  write.url
}

FUNCTION {default.type} { misc }

READ

STRINGS { longest.label }

INTEGERS { number.label longest.label.width }

FUNCTION {initialize.longest.label}
{ "" 'longest.label :=
  #1 'number.label :=
  #0 'longest.label.width :=
}

FUNCTION {longest.label.pass}
{ number.label int.to.str$ 'label :=
  number.label #1 + 'number.label :=
  label width$ longest.label.width >
    { label 'longest.label :=
      label width$ 'longest.label.width :=
    }
    'skip$
  if$
}

EXECUTE {initialize.longest.label}

ITERATE {longest.label.pass}

FUNCTION {begin.bib}
{ preamble$ empty$
    'skip$
    { preamble$ write$ newline$ }
  if$
  "\begin{thebibliography}{"  longest.label  * "}" *
  write$ newline$
  "\expandafter\ifx\csname url\endcsname\relax\def\url#1{\texttt{#1}}\fi"
  write$ newline$
  "\expandafter\ifx\csname urlprefix\endcsname\relax\def\urlprefix{URL }\fi"
  write$ newline$
}

EXECUTE {begin.bib}

EXECUTE {init.state.consts}

ITERATE {call.type$}

FUNCTION {end.bib}
{ newline$
  "\end{thebibliography}" write$ newline$
}

EXECUTE {end.bib}
%% End of customized bst file
%%
%% End of file `elsart-num.bst'.
