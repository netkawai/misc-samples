// INTERNATIONAL AVS CENTRE - WARRANTY DISCLAIMER
// Please read the file DISCLAIMER for conditions associated with this file.
// avs@iavsc.org, www.iavsc.org

flibrary ReadDatamineApps<compile_subs=0> {

   APPS.SingleWindowApp ReadDatamineEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.extract_component.out_obj
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,
               };
            };
         };
      };

      string+Port dir = "$XP_PATH<1>/iac_proj/rd_dmine/data";
      string+Port filenames[4] = {"contours.dm","points.dm","topopt.dm","topotr.dm"};

      IAC_PROJ.ReadDatamine.ReadDatamineMacs.read_datamine read_datamine {
         dir => <-.dir;
         filenames => <-.filenames;
      };

      IAC_PROJ.ReadDatamine.ReadDatamineMacs.extract_field extract_field {
         fldSlider {
            value = 3;
         };
         flds => <-.read_datamine.out_flds;
      };
      MODS.extract_component extract_component {
         in_field => <-.extract_field.out_fld;
         ExtrCompParam {
            component = 2;
         };
      };

   };


};

