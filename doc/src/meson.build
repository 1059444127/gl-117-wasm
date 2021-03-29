pdflatex = find_program('pdflatex', required: false)
if pdflatex.found()
  custom_target(
    'gl-117-pdf',
    input: 'gl-117.tex',
    output: 'gl-117.pdf',
    command: [files('make-pdf.sh'), '@INPUT@', '@OUTDIR@'],
    depend_files: [
      'aerodynamics.tex',
      'basics.tex',
      'buzzard.jpg',
      'crow.jpg',
      'elevation.jpg',
      'falcon.jpg',
      'fly.jpg',
      'forces.jpg',
      'gl-117.jpg',
      'hawk.jpg',
      'heading.jpg',
      'hud.jpg',
      'installation.tex',
      'missile_agm.jpg',
      'missile_df.jpg',
      'missile_ff.jpg',
      'missile_ir.jpg',
      'roll.jpg',
      'squad.tex',
      'swallow.jpg',
      'title.tex',
    ]
  )
endif
