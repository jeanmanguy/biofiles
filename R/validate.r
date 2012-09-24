#' validate a gbRecord database which if referenced by gbFeature or
#' gbFeatureList objects by their '.Dir' slot. 
#' @keywords internal
hasValidDb <- function (object, verbose=TRUE) {
  
  if (!.hasSlot(object, ".Dir")) {
    if (verbose) 
      message("Object has no '.Dir' slot")
    return(FALSE)
  }
  
  if (!file.exists(object@.Dir)) {
    if (verbose)
      message(sprintf("Directory %s does not exist.", sQuote(object@.Dir)))
    return(FASLE)
  }
  
  if (any(idx <- is.na(match(.GBFIELDS, dir(object@.Dir))))) {
    if (verbose)
      message(sprintf("Field(s) %s are missing from database.",
                      sQuote(paste(.GBFIELDS[-idx], collapse=","))))
    return(FALSE)
  }
  
  TRUE
}

#' validate a gbRecord database (i.e. check if the db directory contains
#' all fields).
#' @keywords internal
isValidDb <- function (object, verbose=TRUE) {
  
  if (any(idx <- is.na(match(.GBFIELDS, dir(object@dir))))) {
    if (verbose)
      message(sprintf("Field(s) %s are missing from database.",
                      sQuote(paste(.GBFIELDS[-idx], collapse=", "))))
    return(FALSE)
  }
  
  TRUE
}

# check if the db directory has been moved from the location
# where it was instantiated
hasNewPath <- function (x) {
  !identical(x@dir, x$features@.Dir) 
}

# if yes update the gbFeatureList@.Dir and gbFeature@.Dir
# slots to  point to the current directory.
updateDirectory <- function (db) {
  newPath <- db@dir
  data <- dbFetch(db, "features")
  data <- 
    `slot<-`(data, name=".Dir", check=FALSE, value=newPath)
  data <- 
    `slot<-`(data, name=".Data", check=FALSE,
             value=lapply(data@.Data, `slot<-`, name=".Dir",
                          check=FALSE, value=newPath))
  dbDelete(db, "features")
  dbInsert(db, "features", data)
}


is.compound <- function (x) {
  if (is(x, "gbFeatureList")) {
    return(vapply(x, function (f) not.na(f@location@compound), logical(1)))
  } else if (is(x, "gbFeature")) {
    return(not.na(x@location@compound))
  } else if (is(x, "gbLocation")) {
    return(not.na(x@compound))
  }
}

