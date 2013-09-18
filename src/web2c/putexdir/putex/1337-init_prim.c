Static void initprim(void)
{
  primitive(S(341), assignglue, gluebase);
  primitive(S(342), assignglue, gluebase + baselineskipcode);
  primitive(S(343), assignglue, gluebase + parskipcode);
  primitive(S(344), assignglue, gluebase + abovedisplayskipcode);
  primitive(S(345), assignglue, gluebase + belowdisplayskipcode);
  primitive(S(346), assignglue, gluebase + abovedisplayshortskipcode);
  primitive(S(347), assignglue, gluebase + belowdisplayshortskipcode);
  primitive(S(348), assignglue, gluebase + leftskipcode);
  primitive(S(349), assignglue, gluebase + rightskipcode);
  primitive(S(350), assignglue, gluebase + topskipcode);
  primitive(S(351), assignglue, gluebase + splittopskipcode);
  primitive(S(352), assignglue, gluebase + tabskipcode);
  primitive(S(353), assignglue, gluebase + spaceskipcode);
  primitive(S(354), assignglue, gluebase + xspaceskipcode);
  primitive(S(355), assignglue, gluebase + parfillskipcode);
  primitive(S(356), assignmuglue, gluebase + thinmuskipcode);
  primitive(S(357), assignmuglue, gluebase + medmuskipcode);
  primitive(S(358), assignmuglue, gluebase + thickmuskipcode);
  primitive(S(1026), assigntoks, outputroutineloc);
  primitive(S(1027), assigntoks, everyparloc);
  primitive(S(1028), assigntoks, everymathloc);
  primitive(S(1029), assigntoks, everydisplayloc);
  primitive(S(1030), assigntoks, everyhboxloc);
  primitive(S(1031), assigntoks, everyvboxloc);
  primitive(S(1032), assigntoks, everyjobloc);
  primitive(S(1033), assigntoks, everycrloc);
  primitive(S(1034), assigntoks, errhelploc);   /*:230*/
  primitive(S(1035), assignint, intbase);
  primitive(S(1036), assignint, intbase + tolerancecode);
  primitive(S(1037), assignint, intbase + linepenaltycode);
  primitive(S(1038), assignint, intbase + hyphenpenaltycode);
  primitive(S(1039), assignint, intbase + exhyphenpenaltycode);
  primitive(S(1040), assignint, intbase + clubpenaltycode);
  primitive(S(1041), assignint, intbase + widowpenaltycode);
  primitive(S(1042), assignint, intbase + displaywidowpenaltycode);
  primitive(S(1043), assignint, intbase + brokenpenaltycode);
  primitive(S(1044), assignint, intbase + binoppenaltycode);
  primitive(S(1045), assignint, intbase + relpenaltycode);
  primitive(S(1046), assignint, intbase + predisplaypenaltycode);
  primitive(S(1047), assignint, intbase + postdisplaypenaltycode);
  primitive(S(1048), assignint, intbase + interlinepenaltycode);
  primitive(S(1049), assignint, intbase + doublehyphendemeritscode);
  primitive(S(1050), assignint, intbase + finalhyphendemeritscode);
  primitive(S(1051), assignint, intbase + adjdemeritscode);
  primitive(S(1052), assignint, intbase + magcode);
  primitive(S(1053), assignint, intbase + delimiterfactorcode);
  primitive(S(1054), assignint, intbase + loosenesscode);
  primitive(S(1055), assignint, intbase + timecode);
  primitive(S(1056), assignint, intbase + daycode);
  primitive(S(1057), assignint, intbase + monthcode);
  primitive(S(1058), assignint, intbase + yearcode);
  primitive(S(1059), assignint, intbase + showboxbreadthcode);
  primitive(S(1060), assignint, intbase + showboxdepthcode);
  primitive(S(1061), assignint, intbase + hbadnesscode);
  primitive(S(1062), assignint, intbase + vbadnesscode);
  primitive(S(1063), assignint, intbase + pausingcode);
  primitive(S(1064), assignint, intbase + tracingonlinecode);
  primitive(S(1065), assignint, intbase + tracingmacroscode);
  primitive(S(1066), assignint, intbase + tracingstatscode);
  primitive(S(1067), assignint, intbase + tracingparagraphscode);
  primitive(S(1068), assignint, intbase + tracingpagescode);
  primitive(S(1069), assignint, intbase + tracingoutputcode);
  primitive(S(1070), assignint, intbase + tracinglostcharscode);
  primitive(S(1071), assignint, intbase + tracingcommandscode);
  primitive(S(1072), assignint, intbase + tracingrestorescode);
  primitive(S(1073), assignint, intbase + uchyphcode);
  primitive(S(1074), assignint, intbase + outputpenaltycode);
  primitive(S(1075), assignint, intbase + maxdeadcyclescode);
  primitive(S(1076), assignint, intbase + hangaftercode);
  primitive(S(1077), assignint, intbase + floatingpenaltycode);
  primitive(S(1078), assignint, intbase + globaldefscode);
  primitive(S(333), assignint, intbase + curfamcode);
  primitive(S(1079), assignint, intbase + escapecharcode);
  primitive(S(1080), assignint, intbase + defaulthyphencharcode);
  primitive(S(1081), assignint, intbase + defaultskewcharcode);
  primitive(S(1082), assignint, intbase + endlinecharcode);
  primitive(S(1083), assignint, intbase + newlinecharcode);
  primitive(S(1084), assignint, intbase + languagecode);
  primitive(S(1085), assignint, intbase + lefthyphenmincode);
  primitive(S(1086), assignint, intbase + righthyphenmincode);
  primitive(S(1087), assignint, intbase + holdinginsertscode);
  primitive(S(1088), assignint, intbase + errorcontextlinescode);
  primitive(S(1089), assigndimen, dimenbase);
  primitive(S(1090), assigndimen, dimenbase + mathsurroundcode);
  primitive(S(1091), assigndimen, dimenbase + lineskiplimitcode);
  primitive(S(1092), assigndimen, dimenbase + hsizecode);
  primitive(S(1093), assigndimen, dimenbase + vsizecode);
  primitive(S(1094), assigndimen, dimenbase + maxdepthcode);
  primitive(S(1095), assigndimen, dimenbase + splitmaxdepthcode);
  primitive(S(1096), assigndimen, dimenbase + boxmaxdepthcode);
  primitive(S(1097), assigndimen, dimenbase + hfuzzcode);
  primitive(S(1098), assigndimen, dimenbase + vfuzzcode);
  primitive(S(1099), assigndimen, dimenbase + delimitershortfallcode);
  primitive(S(1100), assigndimen, dimenbase + nulldelimiterspacecode);
  primitive(S(1101), assigndimen, dimenbase + scriptspacecode);
  primitive(S(1102), assigndimen, dimenbase + predisplaysizecode);
  primitive(S(1103), assigndimen, dimenbase + displaywidthcode);
  primitive(S(1104), assigndimen, dimenbase + displayindentcode);
  primitive(S(1105), assigndimen, dimenbase + overfullrulecode);
  primitive(S(1106), assigndimen, dimenbase + hangindentcode);
  primitive(S(1107), assigndimen, dimenbase + hoffsetcode);
  primitive(S(1108), assigndimen, dimenbase + voffsetcode);
  primitive(S(1109), assigndimen, dimenbase + emergencystretchcode);
  primitive(' ', exspace, 0);
  primitive('/', italcorr, 0);
  primitive(S(417), accent, 0);
  primitive(S(1110), advance, 0);
  primitive(S(1111), afterassignment, 0);
  primitive(S(1112), aftergroup, 0);
  primitive(S(1113), begingroup, 0);
  primitive(S(1114), charnum, 0);
  primitive(S(262), csname, 0);
  primitive(S(1115), delimnum, 0);
  primitive(S(1116), divide, 0);
  primitive(S(263), endcsname, 0);
  primitive(S(836), endgroup, 0);
  text(frozenendgroup) = S(836);
  eqtb[frozenendgroup - activebase] = eqtb[curval - activebase];
  primitive(S(1117), expandafter, 0);
  primitive(S(1118), deffont, 0);
  primitive(S(1119), assignfontdimen, 0);
  primitive(S(724), halign, 0);
  primitive(S(863), hrule, 0);
  primitive(S(1120), ignorespaces, 0);
  primitive(S(374), insert_, 0);
  primitive(S(402), mark_, 0);
  primitive(S(913), mathaccent, 0);
  primitive(S(1121), mathcharnum, 0);
  primitive(S(404), mathchoice, 0);
  primitive(S(1122), multiply, 0);
  primitive(S(897), noalign, 0);
  primitive(S(1123), noboundary, 0);
  primitive(S(1124), noexpand, 0);
  primitive(S(388), nonscript, 0);
  primitive(S(900), omit, 0);
  primitive(S(462), setshape, 0);
  primitive(S(761), breakpenalty, 0);
  primitive(S(948), setprevgraf, 0);
  primitive(S(416), radical, 0);
  primitive(S(656), readtocs, 0);
  primitive(S(1125), relax, 256);
  text(frozenrelax) = S(1125);
  eqtb[frozenrelax - activebase] = eqtb[curval - activebase];
  primitive(S(970), setbox, 0);
  primitive(S(604), the, 0);
  primitive(S(463), toksregister, 0);
  primitive(S(403), vadjust, 0);
  primitive(S(1126), valign, 0);
  primitive(S(415), vcenter, 0);
  primitive(S(1127), vrule, 0);   /*:265*/
  primitive(S(760), parend, 256);
  parloc = curval;
  partoken = cstokenflag + parloc;   /*:334*/
  primitive(S(1128), input, 0);
  primitive(S(1129), input, 1);   /*:376*/
  primitive(S(1130), topbotmark, topmarkcode);
  primitive(S(1131), topbotmark, firstmarkcode);
  primitive(S(1132), topbotmark, botmarkcode);
  primitive(S(1133), topbotmark, splitfirstmarkcode);
  primitive(S(1134), topbotmark, splitbotmarkcode);   /*:384*/
  primitive(S(472), register_, intval);
  primitive(S(474), register_, dimenval);
  primitive(S(460), register_, glueval);
  primitive(S(461), register_, muval);   /*:411*/
  primitive(S(1135), setaux, hmode);
  primitive(S(1136), setaux, vmode);
  primitive(S(1137), setpageint, 0);
  primitive(S(1138), setpageint, 1);
  primitive(S(1139), setboxdimen, widthoffset);
  primitive(S(1140), setboxdimen, heightoffset);
  primitive(S(1141), setboxdimen, depthoffset);
  primitive(S(1142), lastitem, intval);
  primitive(S(1143), lastitem, dimenval);
  primitive(S(1144), lastitem, glueval);
  primitive(S(1145), lastitem, inputlinenocode);
  primitive(S(1146), lastitem, badnesscode);   /*:416*/
  primitive(S(1147), convert, numbercode);
  primitive(S(1148), convert, romannumeralcode);
  primitive(S(1149), convert, stringcode);
  primitive(S(1150), convert, meaningcode);
  primitive(S(1151), convert, fontnamecode);
  primitive(S(1152), convert, jobnamecode);   /*:468*/
  primitive(S(658), iftest, ifcharcode);
  primitive(S(1153), iftest, ifcatcode);
  primitive(S(1154), iftest, ifintcode);
  primitive(S(1155), iftest, ifdimcode);
  primitive(S(1156), iftest, ifoddcode);
  primitive(S(1157), iftest, ifvmodecode);
  primitive(S(1158), iftest, ifhmodecode);
  primitive(S(1159), iftest, ifmmodecode);
  primitive(S(1160), iftest, ifinnercode);
  primitive(S(1161), iftest, ifvoidcode);
  primitive(S(1162), iftest, ifhboxcode);
  primitive(S(1163), iftest, ifvboxcode);
  primitive(S(1164), iftest, ifxcode);
  primitive(S(1165), iftest, ifeofcode);
  primitive(S(1166), iftest, iftruecode);
  primitive(S(1167), iftest, iffalsecode);
  primitive(S(1168), iftest, ifcasecode);   /*:487*/
  primitive(S(1169), fiorelse, ficode);
  text(frozenfi) = S(1169);
  eqtb[frozenfi - activebase] = eqtb[curval - activebase];
  primitive(S(664), fiorelse, orcode);
  primitive(S(1170), fiorelse, elsecode);   /*:491*/
  primitive(S(1171), setfont, nullfont);
  text(frozennullfont) = S(1171);
  eqtb[frozennullfont - activebase] = eqtb[curval - activebase];   /*:553*/
  primitive(S(1172), tabmark, spancode);
  primitive(S(737), carret, crcode);
  text(frozencr) = S(737);
  eqtb[frozencr - activebase] = eqtb[curval - activebase];
  primitive(S(1173), carret, crcrcode);
  text(frozenendtemplate) = S(1174);
  text(frozenendv) = S(1174);
  eqtype(frozenendv) = endv;
  equiv(frozenendv) = nulllist;
  eqlevel(frozenendv) = levelone;
  eqtb[frozenendtemplate - activebase] = eqtb[frozenendv - activebase];
  eqtype(frozenendtemplate) = endtemplate;   /*:780*/
  primitive(S(1175), setpagedimen, 0);
  primitive(S(1176), setpagedimen, 1);
  primitive(S(1177), setpagedimen, 2);
  primitive(S(1178), setpagedimen, 3);
  primitive(S(1179), setpagedimen, 4);
  primitive(S(1180), setpagedimen, 5);
  primitive(S(1181), setpagedimen, 6);
  primitive(S(1182), setpagedimen, 7);   /*:983*/
  primitive(S(1183), stop, 0);
  primitive(S(1184), stop, 1);   /*:1052*/
  primitive(S(1185), hskip, skipcode);
  primitive(S(1186), hskip, filcode);
  primitive(S(1187), hskip, fillcode);
  primitive(S(1188), hskip, sscode);
  primitive(S(1189), hskip, filnegcode);
  primitive(S(1190), vskip, skipcode);
  primitive(S(1191), vskip, filcode);
  primitive(S(1192), vskip, fillcode);
  primitive(S(1193), vskip, sscode);
  primitive(S(1194), vskip, filnegcode);
  primitive(S(389), mskip, mskipcode);
  primitive(S(391), kern, explicit);
  primitive(S(393), mkern, muglue);   /*:1058*/
  primitive(S(1195), hmove, 1);
  primitive(S(1196), hmove, 0);
  primitive(S(1197), vmove, 1);
  primitive(S(1198), vmove, 0);
  primitive(S(464), makebox, boxcode);
  primitive(S(1199), makebox, copycode);
  primitive(S(1200), makebox, lastboxcode);
  primitive(S(797), makebox, vsplitcode);
  primitive(S(1201), makebox, vtopcode);
  primitive(S(799), makebox, vtopcode + vmode);
  primitive(S(1202), makebox, vtopcode + hmode);
  primitive(S(1203), leadership, aleaders - 1);
  primitive(S(1204), leadership, aleaders);
  primitive(S(1205), leadership, cleaders);
  primitive(S(1206), leadership, xleaders);   /*:1071*/
  primitive(S(1207), startpar, 1);
  primitive(S(1208), startpar, 0);   /*:1088*/
  primitive(S(1209), removeitem, penaltynode);
  primitive(S(1210), removeitem, kernnode);
  primitive(S(1211), removeitem, gluenode);
  primitive(S(1212), unhbox, boxcode);
  primitive(S(1213), unhbox, copycode);
  primitive(S(1214), unvbox, boxcode);
  primitive(S(1215), unvbox, copycode);   /*:1107*/
  primitive('-', discretionary, 1);
  primitive(S(400), discretionary, 0);   /*:1114*/
  primitive(S(1216), eqno, 0);
  primitive(S(1217), eqno, 1);   /*:1141*/
  primitive(S(405), mathcomp, ordnoad);
  primitive(S(406), mathcomp, opnoad);
  primitive(S(407), mathcomp, binnoad);
  primitive(S(408), mathcomp, relnoad);
  primitive(S(409), mathcomp, opennoad);
  primitive(S(410), mathcomp, closenoad);
  primitive(S(411), mathcomp, punctnoad);
  primitive(S(412), mathcomp, innernoad);
  primitive(S(414), mathcomp, undernoad);
  primitive(S(413), mathcomp, overnoad);
  primitive(S(1218), limitswitch, normal);
  primitive(S(420), limitswitch, limits);
  primitive(S(421), limitswitch, nolimits);   /*:1156*/
  primitive(S(336), mathstyle, displaystyle);
  primitive(S(337), mathstyle, textstyle);
  primitive(S(338), mathstyle, scriptstyle);
  primitive(S(339), mathstyle, scriptscriptstyle);   /*:1169*/
  primitive(S(1219), above, abovecode);
  primitive(S(1220), above, overcode);
  primitive(S(1221), above, atopcode);
  primitive(S(1222), above, delimitedcode);
  primitive(S(1223), above, delimitedcode + overcode);
  primitive(S(1224), above, delimitedcode + atopcode);   /*:1178*/
  primitive(S(418), leftright, leftnoad);
  primitive(S(419), leftright, rightnoad);
  text(frozenright) = S(419);
  eqtb[frozenright - activebase] = eqtb[curval - activebase];   /*:1188*/
  primitive(S(959), prefix, 1);
  primitive(S(961), prefix, 2);
  primitive(S(1225), prefix, 4);
  primitive(S(1226), def, 0);
  primitive(S(1227), def, 1);
  primitive(S(1228), def, 2);
  primitive(S(1229), def, 3);
  primitive(S(1230), let, normal);
  primitive(S(1231), let, normal + 1);   /*:1219*/
  primitive(S(1232), shorthanddef, chardefcode);
  primitive(S(1233), shorthanddef, mathchardefcode);
  primitive(S(1234), shorthanddef, countdefcode);
  primitive(S(1235), shorthanddef, dimendefcode);
  primitive(S(1236), shorthanddef, skipdefcode);
  primitive(S(1237), shorthanddef, muskipdefcode);
  primitive(S(1238), shorthanddef, toksdefcode);   /*:1222*/
  primitive(S(467), defcode, catcodebase);
  primitive(S(471), defcode, mathcodebase);
  primitive(S(468), defcode, lccodebase);
  primitive(S(469), defcode, uccodebase);
  primitive(S(470), defcode, sfcodebase);
  primitive(S(473), defcode, delcodebase);
  primitive(S(266), deffamily, mathfontbase);
  primitive(S(267), deffamily, mathfontbase + scriptsize);
  primitive(S(268), deffamily, mathfontbase + scriptscriptsize);
  primitive(S(787), hyphdata, 0);
  primitive(S(774), hyphdata, 1);   /*:1250*/
  primitive(S(1239), assignfontint, 0);
  primitive(S(1240), assignfontint, 1);   /*:1254*/
  primitive(S(281), setinteraction, batchmode);
  primitive(S(282), setinteraction, nonstopmode);
  primitive(S(283), setinteraction, scrollmode);
  primitive(S(1241), setinteraction, errorstopmode);   /*:1262*/
  primitive(S(1242), instream, 1);
  primitive(S(1243), instream, 0);   /*:1272*/
  primitive(S(1244), message, 0);
  primitive(S(1245), message, 1);   /*:1277*/
  primitive(S(1246), caseshift, lccodebase);
  primitive(S(1247), caseshift, uccodebase);   /*:1286*/
  primitive(S(1248), xray, showcode);
  primitive(S(1249), xray, showboxcode);
  primitive(S(1250), xray, showthecode);
  primitive(S(1251), xray, showlists);   /*:1291*/
  primitive(S(378), extension, opennode);
  primitive(S(379), extension, writenode);
  writeloc = curval;
  primitive(S(380), extension, closenode);
  primitive(S(381), extension, specialnode);
  primitive(S(1252), extension, immediatecode);
  primitive(S(382), extension, setlanguagecode);   /*:1344*/
}
