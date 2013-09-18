void print_cmd_chr(quarterword cmd, halfword chr_code)
{
    switch (cmd) {
        case leftbrace: chr_cmd(S(1256)); break;
        case rightbrace: chr_cmd(S(1257)); break;
        case mathshift: chr_cmd(S(1258)); break;
        case macparam: chr_cmd(S(1259)); break;
        case supmark: chr_cmd(S(1260)); break;
        case submark: chr_cmd(S(1261)); break;
        case endv: print(S(1262)); break;
        case spacer: chr_cmd(S(1263)); break;
        case letter: chr_cmd(S(1264)); break;
        case otherchar: chr_cmd(S(1265)); break;
        case assignglue:
        case assignmuglue:
                        if (chr_code < skip_base)
                            print_skip_param(chr_code - glue_base);
                        else if (chr_code < muskipbase) {
                            print_esc(S(460));
                            printint(chr_code - skipbase);
                        } else {
                            print_esc(S(461));
                            printint(chr_code - muskipbase);
                        }
                        break;
        case assigntoks:
            if (chr_code >= toksbase) {
                            print_esc(S(463));
                            printint(chr_code - toksbase);
                        } else {
                            switch (chr_code) {
                                case outputroutineloc: print_esc(S(1026)); break;
                                case everyparloc: print_esc(S(1027)); break;
                                case everymathloc: print_esc(S(1028)); break;
                                case everydisplayloc: print_esc(S(1029)); break;
                                case everyhboxloc: print_esc(S(1030)); break;
                                case everyvboxloc: print_esc(S(1031)); break;
                                case everyjobloc: print_esc(S(1032)); break;
                                case everycrloc: print_esc(S(1033)); break;
                                default: print_esc(S(1034)); break;
                            }
                        }
                        break;
        case assignint:
                        if (chr_code < countbase)
                            printparam(chr_code - intbase);
                        else {
                            print_esc(S(472));
                            printint(chr_code - countbase);
                        } /*:239*/
                        break;
        case assigndimen: /*:249*/
                        if (chr_code < scaledbase)
                            printlengthparam(chr_code - dimenbase);
                        else {
                            print_esc(S(474));
                            printint(chr_code - scaledbase);
                        }
                        break;
        case accent: print_esc(S(417)); break;
        case advance: print_esc(S(1110)); break;
        case afterassignment: print_esc(S(1111)); break;
        case aftergroup: print_esc(S(1112)); break;
        case assignfontdimen: print_esc(S(1119)); break;
        case begingroup: print_esc(S(1113)); break;
        case breakpenalty: print_esc(S(761)); break;
        case charnum: print_esc(S(1114)); break;
        case csname: print_esc(S(262)); break;
        case deffont: print_esc(S(1118)); break;
        case delimnum: print_esc(S(1115)); break;
        case divide: print_esc(S(1116)); break;
        case endcsname: print_esc(S(263)); break;
        case endgroup: print_esc(S(836)); break;
        case exspace: print_esc(' '); break;
        case expandafter: print_esc(S(1117)); break;
        case halign: print_esc(S(724)); break;
        case hrule: print_esc(S(863)); break;
        case ignorespaces: print_esc(S(1120)); break;
        case insert_: print_esc(S(374)); break;
        case italcorr: print_esc('/'); break;
        case mark_: print_esc(S(402)); break;
        case mathaccent: print_esc(S(913)); break;
        case mathcharnum: print_esc(S(1121)); break;
        case mathchoice: print_esc(S(404)); break;
        case multiply: print_esc(S(1122)); break;
        case noalign: print_esc(S(897)); break;
        case noboundary: print_esc(S(1123)); break;
        case noexpand: print_esc(S(1124)); break;
        case nonscript: print_esc(S(388)); break;
        case omit: print_esc(S(900)); break;
        case radical: print_esc(S(416)); break;
        case readtocs: print_esc(S(656)); break;
        case relax: print_esc(S(1125)); break;
        case setbox: print_esc(S(970)); break;
        case setprevgraf: print_esc(S(948)); break;
        case setshape: print_esc(S(462)); break;
        case the: print_esc(S(604)); break;
        case toksregister: print_esc(S(463)); break;
        case vadjust: print_esc(S(403)); break;
        case valign: print_esc(S(1126)); break;
        case vcenter: print_esc(S(415)); break;
        case vrule: print_esc(S(1127)); break;
        case parend: print_esc(S(760)); break;
        case input: /*:377*/
                     if (chr_code == 0)
                         print_esc(S(1128));
                     else
                         print_esc(S(1129));
                     break;
        case topbotmark: /*:385*/
                     switch (chr_code) {
                         case firstmarkcode: print_esc(S(1131)); break;
                         case botmarkcode: print_esc(S(1132)); break;
                         case splitfirstmarkcode: print_esc(S(1133)); break;
                         case splitbotmarkcode: print_esc(S(1134)); break;
                         default: print_esc(S(1130)); break;
                     }
                     break;
        case register_: /*:412*/
                     if (chr_code == intval)
                         print_esc(S(472));
                     else if (chr_code == dimenval)
                         print_esc(S(474));
                     else if (chr_code == glueval)
                         print_esc(S(460));
                     else
                         print_esc(S(461));
                     break;
        case setaux:
                     if (chr_code == vmode)
                         print_esc(S(1136));
                     else
                         print_esc(S(1135));
                     break;
        case setpageint:
                     if (chr_code == 0)
                         print_esc(S(1137));
                     else
                         print_esc(S(1138));
                     break;
        case setboxdimen:
                     if (chr_code == widthoffset)
                         print_esc(S(1139));
                     else {
                         if (chr_code == heightoffset)
                             print_esc(S(1140));
                         else
                             print_esc(S(1141));
                     }
                     break;

        case lastitem: /*:417*/
                     switch (chr_code) {
                         case intval: print_esc(S(1142)); break;
                         case dimenval: print_esc(S(1143)); break;
                         case glueval: print_esc(S(1144)); break;
                         case inputlinenocode: print_esc(S(1145)); break;
                         default: print_esc(S(1146)); break;
                     }
                     break;
        case convert: /*:469*/
                     switch (chr_code) {
                         case numbercode: print_esc(S(1147)); break;
                         case romannumeralcode: print_esc(S(1148)); break;
                         case stringcode: print_esc(S(1149)); break;
                         case meaningcode: print_esc(S(1150)); break;
                         case fontnamecode: print_esc(S(1151)); break;
                         default: print_esc(S(1152)); break;
                     }
                     break;
        case iftest: /*:488*/
                     switch (chr_code) {
                         case ifcatcode: print_esc(S(1153)); break;
                         case ifintcode: print_esc(S(1154)); break;
                         case ifdimcode: print_esc(S(1155)); break;
                         case ifoddcode: print_esc(S(1156)); break;
                         case ifvmodecode: print_esc(S(1157)); break;
                         case ifhmodecode: print_esc(S(1158)); break;
                         case ifmmodecode: print_esc(S(1159)); break;
                         case ifinnercode: print_esc(S(1160)); break;
                         case ifvoidcode: print_esc(S(1161)); break;
                         case ifhboxcode: print_esc(S(1162)); break;
                         case ifvboxcode: print_esc(S(1163)); break;
                         case ifxcode: print_esc(S(1164)); break;
                         case ifeofcode: print_esc(S(1165)); break;
                         case iftruecode: print_esc(S(1166)); break;
                         case iffalsecode: print_esc(S(1167)); break;
                         case ifcasecode: print_esc(S(1168)); break;
                         default: print_esc(S(658)); break;
                     }
                     break;
        case fiorelse: /*:492*/
                     if (chr_code == ficode)
                         print_esc(S(1169));
                     else if (chr_code == orcode)
                         print_esc(S(664));
                     else
                         print_esc(S(1170));
                     break;
        case tabmark:
                     if (chr_code == spancode)
                         print_esc(S(1172));
                     else {
                         chr_cmd(S(1266));
                     }
                     break;
        case carret: /*:781*/
                     if (chr_code == crcode)
                         print_esc(S(737));
                     else
                         print_esc(S(1173));
                     break;
        case setpagedimen: /*:984*/
                     switch (chr_code) {
                         case 0: print_esc(S(1175)); break;
                         case 1: print_esc(S(1176)); break;
                         case 2: print_esc(S(1177)); break;
                         case 3: print_esc(S(1178)); break;
                         case 4: print_esc(S(1179)); break;
                         case 5: print_esc(S(1180)); break;
                         case 6: print_esc(S(1181)); break;
                         default: print_esc(S(1182)); break;
                     }
                     break;
        case stop: /*:1053*/
                     if (chr_code == 1)
                         print_esc(S(1184));
                     else
                         print_esc(S(1183));
                     break;
        case hskip:
                     switch (chr_code) {
                         case skipcode: print_esc(S(1185)); break;
                         case filcode: print_esc(S(1186)); break;
                         case fillcode: print_esc(S(1187)); break;
                         case sscode: print_esc(S(1188)); break;
                         default: print_esc(S(1189)); break;
                     }
                     break;
        case vskip:
                     switch (chr_code) {
                         case skipcode: print_esc(S(1190)); break;
                         case filcode: print_esc(S(1191)); break;
                         case fillcode: print_esc(S(1192)); break;
                         case sscode: print_esc(S(1193)); break;
                         default: print_esc(S(1194)); break;
                     }
                     break;
        case mskip: print_esc(S(389)); break;
        case kern: print_esc(S(391)); break;
        case mkern: print_esc(S(393)); break;
        case hmove:
                    if (chr_code == 1)
                        print_esc(S(1195));
                    else
                        print_esc(S(1196));
                    break;
        case vmove:
                    if (chr_code == 1)
                        print_esc(S(1197));
                    else
                        print_esc(S(1198));
                    break;
        case makebox:
                    switch (chr_code) {
                        case boxcode: print_esc(S(464)); break;
                        case copycode: print_esc(S(1199)); break;
                        case lastboxcode: print_esc(S(1200)); break;
                        case vsplitcode: print_esc(S(797)); break;
                        case vtopcode: print_esc(S(1201)); break;
                        case vtopcode + vmode: print_esc(S(799)); break;
                        default: print_esc(S(1202)); break;
                    }
                    break;
        case leadership: /*:1072*/
                    if (chr_code == aleaders)
                        print_esc(S(1204));
                    else if (chr_code == cleaders)
                        print_esc(S(1205));
                    else if (chr_code == xleaders)
                        print_esc(S(1206));
                    else
                        print_esc(S(1203));
                    break;
        case startpar: /*:1089*/
                    if (chr_code == 0)
                        print_esc(S(1208));
                    else
                        print_esc(S(1207));
                    break;
        case removeitem:
                    if (chr_code == gluenode)
                        print_esc(S(1211));
                    else if (chr_code == kernnode)
                        print_esc(S(1210));
                    else
                        print_esc(S(1209));
                    break;
        case unhbox:
                    if (chr_code == copycode)
                        print_esc(S(1213));
                    else
                        print_esc(S(1212));
                    break;
        case unvbox: /*:1108*/
                    if (chr_code == copycode)
                        print_esc(S(1215));
                    else
                        print_esc(S(1214));
                    break;
        case discretionary: /*:1115*/
                    if (chr_code == 1)
                        print_esc('-');
                    else
                        print_esc(S(400));
                    break;
        case eqno: /*:1143*/
                    if (chr_code == 1)
                        print_esc(S(1217));
                    else
                        print_esc(S(1216));
                    break;
        case mathcomp:
                    switch (chr_code) {
                        case ordnoad: print_esc(S(405)); break;
                        case opnoad: print_esc(S(406)); break;
                        case binnoad: print_esc(S(407)); break;
                        case relnoad: print_esc(S(408)); break;
                        case opennoad: print_esc(S(409)); break;
                        case closenoad: print_esc(S(410)); break;
                        case punctnoad: print_esc(S(411)); break;
                        case innernoad: print_esc(S(412)); break;
                        case undernoad: print_esc(S(414)); break;
                        default: print_esc(S(413)); break;
                    }
                    break;
        case limitswitch: /*:1157*/
                    if (chr_code == limits)
                        print_esc(S(420));
                    else if (chr_code == nolimits)
                        print_esc(S(421));
                    else
                        print_esc(S(1218));
                    break;
        case mathstyle: /*:1170*/
                    printstyle(chr_code);
                    break;
        case above: /*:1179*/
                    switch (chr_code) {
                        case overcode: print_esc(S(1220)); break;
                        case atopcode: print_esc(S(1221)); break;
                        case delimitedcode: print_esc(S(1222)); break;
                        case delimitedcode + overcode: print_esc(S(1223)); break;
                        case delimitedcode + atopcode: print_esc(S(1224)); break;
                        default: print_esc(S(1219)); break;
                    }
                    break;
        case leftright: /*:1189*/
                    if (chr_code == leftnoad)
                        print_esc(S(418));
                    else
                        print_esc(S(419));
                    break;
        case prefix:
                    if (chr_code == 1)
                        print_esc(S(959));
                    else if (chr_code == 2)
                        print_esc(S(961));
                    else
                        print_esc(S(1225));
                    break;
        case def: /*:1209*/
                    if (chr_code == 0)
                        print_esc(S(1226));
                    else if (chr_code == 1)
                        print_esc(S(1227));
                    else if (chr_code == 2)
                        print_esc(S(1228));
                    else
                        print_esc(S(1229));
                    break;
        case let: /*:1220*/
                    if (chr_code != normal)
                        print_esc(S(1231));
                    else
                        print_esc(S(1230));
                    break;
        case shorthanddef:
                    switch (chr_code) {
                        case chardefcode: print_esc(S(1232)); break;
                        case mathchardefcode: print_esc(S(1233)); break;
                        case countdefcode: print_esc(S(1234)); break;
                        case dimendefcode: print_esc(S(1235)); break;
                        case skipdefcode: print_esc(S(1236)); break;
                        case muskipdefcode: print_esc(S(1237)); break;
                        default: print_esc(S(1238)); break;
                    }
                    break;
        case chargiven:
                    print_esc(S(1114));
                    printhex(chr_code);
                    break;
        case mathgiven:
                    print_esc(S(1121));
                    printhex(chr_code);
                    break;
        case defcode:
                    if (chr_code == catcodebase)
                        print_esc(S(467));
                    else if (chr_code == mathcodebase)
                        print_esc(S(471));
                    else if (chr_code == lccodebase)
                        print_esc(S(468));
                    else if (chr_code == uccodebase)
                        print_esc(S(469));
                    else if (chr_code == sfcodebase)
                        print_esc(S(470));
                    else
                        print_esc(S(473));
                    break;
        case deffamily: printsize(chr_code - mathfontbase); break;
        case hyphdata: /*:1251*/
                        if (chr_code == 1)
                            print_esc(S(774));
                        else
                            print_esc(S(787));
                        break;
        case assignfontint: /*:1255*/
                        if (chr_code == 0)
                            print_esc(S(1239));
                        else
                            print_esc(S(1240));
                        break;
        case setfont:
                        print(S(1267));
                        slowprint(get_fontname(chr_code));
                        if (get_fontsize(chr_code) != get_fontdsize(chr_code)) {
                            print(S(642));
                            printscaled(get_fontsize(chr_code));
                            print(S(459));
                        }
                        break;
        case setinteraction: /*:1263*/
                        switch (chr_code) {
                            case batchmode: print_esc(S(281)); break;
                            case nonstopmode: print_esc(S(282)); break;
                            case scrollmode: print_esc(S(283)); break;
                            default: print_esc(S(1241)); break;
                        }
                        break;
        case instream: /*:1273*/
                        if (chr_code == 0)
                            print_esc(S(1243));
                        else
                            print_esc(S(1242));
                        break;
        case message: /*:1278*/
                        if (chr_code == 0)
                            print_esc(S(1244));
                        else
                            print_esc(S(1245));
                        break;
        case caseshift: /*:1287*/
                        if (chr_code == lccodebase)
                            print_esc(S(1246));
                        else
                            print_esc(S(1247));
                        break;
        case xray: /*:1292*/
                        switch (chr_code) {
                            case showboxcode: print_esc(S(1249)); break;
                            case showthecode: print_esc(S(1250)); break;
                            case showlists: print_esc(S(1251)); break;
                            default: print_esc(S(1248)); break;
                        }
                        break;
        case undefinedcs: print(S(1268)); break;
        case call: print(S(1269)); break;
        case longcall: print_esc(S(1270)); break;
        case outercall: print_esc(S(1271)); break;
        case longoutercall: print_esc(S(959)); print_esc(S(1271)); break;
        case endtemplate: print_esc(S(1272)); break; /*1346:*/
        case extension:
                          switch (chr_code) {
                              case opennode: print_esc(S(378)); break;
                              case writenode: print_esc(S(379)); break;
                              case closenode: print_esc(S(380)); break;
                              case specialnode: print_esc(S(381)); break;
                              case immediatecode: print_esc(S(1252)); break;
                              case setlanguagecode: print_esc(S(382)); break;
                              default: print(S(1273)); break;
                          }/*:1346*/
                          break;
        default: print(S(1274)); break;
    }
}

