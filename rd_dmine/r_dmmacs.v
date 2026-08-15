// INTERNATIONAL AVS CENTRE - WARRANTY DISCLAIMER
// Please read the file DISCLAIMER for conditions associated with this file.
// avs@iavsc.org, www.iavsc.org

flibrary ReadDatamineMacs<compile_subs=0> {

   macro ReadDatamineUI {
      IAC_PROJ.ReadDatamine.ReadDatamineMods.ReadDatamineParams &params<NEportLevels={2,1}>;

      UImod_panel modPanel {
         title => name_of(<-.<-,1);
      };

      UItoggle pointToggle {
         parent => <-.modPanel;
         label => "Import model as points";
         set => params.model_as_point;
         width => parent.width;
      };

      UIlabel scaleLabel {
         parent => <-.modPanel;
         label => "Model block scale";
         x => pointToggle.x;
         y => pointToggle.y + pointToggle.height + 16;
         alignment = "left";
      };
      UIfield scaleField {
         parent => <-.modPanel;
         x => scaleLabel.x + scaleLabel.width + 5;
         y => scaleLabel.y - (height / 4);
         width => parent.width - x;
         min = 0.;
         value => params.model_scale;
      };

      UIlabel errLabel {
         parent => <-.modPanel;
         visible => <-.params.err;
         x => pointToggle.x;
         y => scaleLabel.y + scaleLabel.height + 15;
         width => parent.width;
         alignment = 0;
         label = "Read Error:";
         color {
            foregroundColor = "red";
         };
      };
      UIlabel errLabelMsg {
         parent => <-.modPanel;
         visible => <-.params.err;
         x => pointToggle.x;
         y => errLabel.y + errLabel.height + 5;
         width => parent.width;
         alignment = 0;
         label => <-.params.err_str;
      };

   };


   macro ReadDatamineFunc {
      IAC_PROJ.ReadDatamine.ReadDatamineMods.ReadDatamineParams &params<NEportLevels={2,1}>;

      IAC_PROJ.ReadDatamine.ReadDatamineMods.ReadDatamineCore ReadDatamineCore {
         params => <-.params;
      };

      GDM.DataObject DatamineObj {
         in => <-.ReadDatamineCore.fld;
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_flds => .ReadDatamineCore.fld;
      olink out_obj => .DatamineObj.obj;
   };


   macro read_datamine {
      ilink dir;
      mlink filenames<NEportLevels={2,1}>;

      IAC_PROJ.ReadDatamine.ReadDatamineMods.ReadDatamineParams params {
         dir => <-.dir;
         filenames => <-.filenames;
         model_as_point = 1;
         model_scale = 1;
         err = 0;
         err_str = "";
      };

      IAC_PROJ.ReadDatamine.ReadDatamineMacs.ReadDatamineUI ReadDatamineUI {
         params => <-.params;
      };

      IAC_PROJ.ReadDatamine.ReadDatamineMacs.ReadDatamineFunc ReadDatamineFunc {
         params => <-.params;
         DatamineObj {
            Obj {
               name => name_of(<-.<-.<-.<-);
            };
         };
      };

      olink out_flds => .ReadDatamineFunc.out_flds;
      olink out_obj => .ReadDatamineFunc.out_obj;
   };



   macro extract_field {
      mlink flds<NEportLevels={2,1}>;

      UImod_panel UImod_panel {
         title => name_of(<-.<-,1);
      };
      UIlabel extrLabel {
         parent => <-.UImod_panel;
         label => "Extract field from array";
         x = 0;
         y = 0;
         width = parent.width;
         alignment = "center";
      };
      UIslider fldSlider {
         parent => <-.UImod_panel;
         value = 0.;
         title => "Field";
         x => <-.extrLabel.x;
         y => <-.extrLabel.y + <-.extrLabel.height + 8;
         width => (parent.width - (2 * 8));
         min = 0.;
         max => switch(((array_size(flds) > 0) + 1),0,(array_size(flds) - 1));
         mode = "integer";
      };


      int+Port n => .fldSlider.value;

      GMOD.parse_v parse_v {
         v_commands => switch( ((array_size(flds) > 0) + 1),
                                    "obj.in => ;",
                                    switch( ((n >= array_size(flds)) + 1),
                                       "obj.in => ; obj.in => <-.flds[<-.n];",
                                       "obj.in => ; n = 0;"
                                    )
                             );
         trigger = array_size(flds);
         on_inst = 0;
         relative = ".";
      };

      GDM.DataObject obj {
         Obj {
            name => name_of(<-.<-.<-);
         };
      };

      olink out_fld => obj.in;
      olink out_obj => obj.obj;
   };


};

