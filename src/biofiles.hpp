#ifndef __BIOFILES__ // make sure the header is included only once
#define __BIOFILES__

#include <Rcpp.h>
#include <string>
#include <vector>
#include <boost/regex.hpp>

struct BaseSpan {
    std::vector<int> range;
    std::vector<bool> fuzzy;
    int strand;
    std::string accession;
    bool remote;
    char type;
};

void parse_simple_span(
    BaseSpan& bs,
    std::string& simple_span, 
    std::string& accession );
    
void parse_gb_location(
    Rcpp::S4 &gb_location,
    std::string &gb_base_span,
    std::string &accession );

void parse_gb_feature_table(
    Rcpp::S4& gb_feature,
    const std::vector<std::string>& feature_string,
    std::string& accession );

Rcpp::CharacterVector parse_gb_qualifiers( 
    const std::vector<std::string>& qualifiers );

/*
 * The notation '55^56' describes a site between two adjoining nucleotides,
 * such as endonucleolytic cleavage sites. The permitted formats for 
 * this descriptor are n^n+1, or, for circular molecules, n^1, where "n" 
 * is the full length of the molecule.
 *
 * As of 2006 the use of the notation '12.21' to indicate a single base taken
 * from between the indicated points is deprecated. We are not gonna support
 * it.
 *
 * Sequence spans are indicated by e.g. '34..456'. The '<' and '>' symbols are 
 * used with the starting and ending base numbers to indicate that an end 
 * point is beyond the specified base number.
 *
 * A location in a remote entry is specified by e.g. 'J12345.1:1..15'.
 *
 * Complemented and compound locations:
 *   complement(location)
 *   join(location,location,...)
 *   order(location,location,...)
 *   bond(location,location,...) only in GenPept files
 */

// empty
static const std::string EMPTY("");

// whitespace
static const boost::regex WHITESPACE("\\s+");

// complement
static const boost::regex COMPL("^complement");

// compound
static const boost::regex CMPND("(join|order|bond)");

// BETWEEN_BASES type
static const boost::regex BETWEEN_BASES("\\d+\\^\\d+");

// split bases 
static const boost::regex BASESPLIT(
    "([<>]?\\d+)(\\.\\.|\\^)?([<>]?\\d+)?");

// remote accession
static const boost::regex RA(
    "[a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?\\:");

// simple location possibly with remote accession
static const boost::regex RASL(
    "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
    "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)");

// Possibly complemented simple location
static const boost::regex PCSL(
    "^("
        "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
        "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
    "|"
        "complement\\("
            "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
            "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
        "\\)"
    ")$");

// Simple locations in compound
static const boost::regex SLC(
    "("
        "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
        "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
    "|"
        "complement\\("
            "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
            "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
        "\\)"
    ")"
    "(,"
        "("
            "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
            "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
        "|"
            "complement\\("
                "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
            "\\)"
        ")"
    ")*");

// compound location
static const boost::regex CL(
    "(join|order|bond)\\("
        "("
            "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
            "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
        "|"
            "complement\\("
                "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
            "\\)"
        ")"
        "(,"
            "("
                "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
                "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
            "|"
                "complement\\("
                    "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                    "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                "\\)"
            ")"
        ")*"
    "\\)");

// possibly complemented compound location
static const boost::regex PCCL(
    "^("
        "(join|order|bond)\\("
            "("
                "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
                "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
            "|"
                "complement\\("
                    "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                    "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                "\\)"
            ")"
            "(,"
                "("
                      "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
                      "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                "|"
                    "complement\\("
                        "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                        "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                    "\\)"
                ")"
            ")*"
        "\\)"
    "|"
        "complement\\("
            "(join|order|bond)\\("
                "("
                    "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
                    "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                "|"
                    "complement\\("
                        "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                        "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                    "\\)"
                ")"
                "(,"
                    "("
                        "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"  
                        "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                    "|"
                        "complement\\("
                            "(([a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z0-9]+)?)\\:)?"
                            "([<>]?\\d+\\.\\.[<>]?\\d+|\\d+\\^\\d+|[<>]?\\d+)"
                        "\\)"
                    ")"
                ")*"
            "\\)"
        "\\)"
    ")$");

#endif // __BIOFILES__