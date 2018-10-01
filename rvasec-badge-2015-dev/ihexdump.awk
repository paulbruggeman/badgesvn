BEGIN {
   for(i = 0; i < 256; i++) {
      c = sprintf("%c", i);
      printable[i] = (c ~ /[[:graph:]]/) ? c : ".";
   }
}

/^\:/ {
   reclen = strtonum("0x" substr($0, 2, 2));
   if(reclen > maxreclen) {
      maxreclen = reclen;
   }
   offset = strtonum("0x" substr($0, 4, 4));
   rectype = strtonum("0x" substr($0, 8, 2));
   line++;
   if(rectype == 0) {         # Data Record
      start[line] = sprintf("%5d %08X %s", line, sba + lba + offset, "DATA");
      for(i = 0; i < reclen; i++) {
         data[line] = data[line] " " substr($0, 10 + i * 2, 2);
         c = strtonum("0x" substr($0, 10 + i * 2, 2));
         ascii[line] = ascii[line] printable[c];
      }
   } else if(rectype == 1) {  # End of File
      start[line] = sprintf("%5d %8s %s", line, "", "EOF");
   } else if(rectype == 2) {  # Extended segment address record 
      sba = strtonum("0x" substr($0, 10, 4)) * 16;
      start[line] = sprintf("%5d %08X %s", line, sba, "ESAR");
   } else if(rectype == 3) {  # Start segment address record       
      start[line] = sprintf("%5d %08X %s", line, strtonum("0x" substr($0, 10, 8)), "SSAR");
   } else if(rectype == 4) {  # Extended linear address record
      lba = strtonum("0x" substr($0, 10, 4)) * 65536;
      start[line] = sprintf("%5d %08X %s", line, lba, "ELAR");
   } else if(rectype == 5) { # Start linear address record 
      start[line] = sprintf("%5d %08X %s", line, strtonum("0x" substr($0, 10, 8)), "SLAR");
   }
}

END {
      print("Line  Addr     Rec  Data");
      for(i = 1; i < line + 1; i++) {
         printf(start[i]);
         printf(data[i]);
         printf("%*s%s\n", (maxreclen * 3) - length(data[i]) + 1, "", ascii[i]);
      }
}
 
