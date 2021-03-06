% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/utils.R
\name{linebreak}
\alias{linebreak}
\title{Format paragraphs}
\usage{
linebreak(s, width = getOption("width") - 2, indent = 0, offset = 0,
  split = " ", FORCE = FALSE, FULL_FORCE = FALSE)
}
\arguments{
\item{s}{a character vector.}

\item{width}{a positive integer giving the column for inserting
linefeeds}

\item{indent}{an integer giving the indentation of the first line of
the paragraph; negative values of \code{indent} are allowed and reduce
the width for the first line by that value.}

\item{offset}{a non-negative integer giving the indentation of all
but the first line.}

\item{split}{regular expression used for splitting. Defaults to whitespace.}

\item{FORCE}{Words are force-split if the available width is too small.}

\item{FULL_FORCE}{Lines are split exactly at the specified width
irrespective of whether there is whitespace or not.}
}
\value{
a character vector
}
\description{
Similar to \code{\link{strwrap}} but returns a single string with
linefeeds inserted
}
\keyword{internal}
