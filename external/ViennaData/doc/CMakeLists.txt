if(BUILD_DOXYGEN_DOCS)
   configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
   add_custom_command(OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html/index.html
      COMMAND ${DOXYGEN_EXECUTABLE}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      VERBATIM)
   # Runs only once
   add_custom_target(apidoc-all ALL
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html/index.html)
   # For manual updates
   add_custom_target(apidoc
      COMMAND ${DOXYGEN_EXECUTABLE}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      VERBATIM)
endif()

if(BUILD_MANUAL)
   set(MANUAL_SRCS
      manual/advanced-usage.tex
      manual/basic-usage.tex
      manual/benchmarks.tex
      manual/changelogs.tex
      manual/contributors.tex
      manual/cover.tex
      manual/design-decisions.tex
      manual/figures/logo_px200.eps
      manual/figures/note.eps
      manual/figures/tip.eps
      manual/figures/TU_Signet_CMYK.eps
      manual/IEEEtran_v1.13.bst
      manual/installation.tex
      manual/internals.tex
      manual/introduction.tex
      manual/keywords.tex
      manual/license.tex
      manual/Makefile
      manual/pitfalls.tex
      manual/setup.tex
      manual/use-cases.tex
      manual/viennadata.bib
      manual/viennadata.tex)

   set(MANUAL_OBJS
      "${CMAKE_CURRENT_BINARY_DIR}/viennadata.pdf"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.bbl"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.blg"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.dvi"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.idx"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.log"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.out"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.ps"
      "${CMAKE_CURRENT_BINARY_DIR}/manual/viennadata.toc")
   foreach(f IN LISTS MANUAL_SRCS)
      configure_file(${f} ${CMAKE_CURRENT_BINARY_DIR}/${f} COPYONLY)
      if(f MATCHES "(.*)\\.tex$")
         list(APPEND MANUAL_OBJS
            "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_MATCH_1}.aux")
      endif()
   endforeach()

   add_custom_command(OUTPUT ${MANUAL_OBJS}
      COMMAND ${LATEX_COMPILER} viennadata.tex
      COMMAND ${LATEX_COMPILER} viennadata.tex
      COMMAND ${BIBTEX_COMPILER} viennadata
      COMMAND ${LATEX_COMPILER} viennadata.tex
      COMMAND ${BIBTEX_COMPILER} viennadata
      COMMAND ${LATEX_COMPILER} viennadata.tex
      COMMAND ${DVIPS_CONVERTER} -Ppdf -G0 -ta4 viennadata.dvi
      COMMAND ${PS2PDF_CONVERTER} -dPDFSETTINGS=/prepress
         -dCompatibilityLevel=1.3 -dMAxSubsetPct=100 -dSubsetFonts=true
         -dEmbedAllFonts=true -sPAPERSIZE=a4 -dAutoRotatePages=/None
         -dOptimize=true viennadata.ps ../viennadata.pdf
      DEPENDS ${MANUAL_SRCS}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual
      COMMENT "Generating manual viennadata.pdf"
      VERBATIM)

   add_custom_target(manual ALL
      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/viennadata.pdf")
endif()
