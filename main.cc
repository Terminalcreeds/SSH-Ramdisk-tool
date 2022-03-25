

// Copyright (C) 2022 Dr238

#include "include/Needs.hpp"
#include "include/Components.hpp"


int main(int argc, char **argv){

 if(argc < 2){
  std::cout << "Example: " << argv[0] << " -d iPad7,5 -i 14.5.1 -b j71bap -s bla/blob.shsh2 [-p | -v | -c]" << std::endl;
   return -1;
 }

 for(size_t i = 0; i < argc; i++)
{
   if(strcmp(argv[i], "-h") == 0) {
     load::Help();
     return -1;
   }
   if(strcmp(argv[i], "-d") == 0) {
     identifier = argv[i + 1];
   }
   if(strcmp(argv[i], "-i") == 0) {
     version = argv[i + 1];
   }
   if(strcmp(argv[i], "-b") == 0) {
     board = argv[i + 1];
   }
   if(strcmp(argv[i], "-s") == 0) {
     blob = argv[i + 1];
   }
   if(strcmp(argv[i], "-p") == 0) {
     load::Pwndevice();
   }
   if(strcmp(argv[i], "-v") == 0) {
     verbose = true;
   }
   if(strcmp(argv[i], "-c") == 0) {
     customlogo = true;
     logo = argv[i + 1];
   }
}

//std::cout << identifier << " " << version << " " << board << " " << blob << " " << verbose << " " << customlogo << '\n';
std::ifstream dirchk((std::string("Patched_") + identifier + "_" + version + "/ramdisk").c_str());
std::ifstream chk((std::string("Patched_") + identifier + "_" + version + "/iBSS.img4").c_str());
std::ifstream chklogo((std::string("Patched_") + identifier + "_" + version + "/logo.img4").c_str());
if(chk && verbose == 1) {
  std::cout << "Going straight to booting verbose. Press enter to boot..";
  std::cin.get();
  load::VerboseBoot(version);
  return 0;
}
else if(chklogo && customlogo == true) {
  std::cout << "Patched dir exists. Press enter to boot..";
  std::cin.get();
  load::BootWithCustomLogo(version);
  return 0;
}
 else if(dirchk){
   std::cout << "[i] Patched dir already exists. Going straight to booting.." << std::endl;
   std::cout << RED << "You ready to boot? y/n " << RESET;
   std::cin >> load::inpt.usrin;
   if(load::inpt.usrin == "y") {
     load::Ramdisk(version);
     return 0;
   }
   else if(load::inpt.usrin == "n") {
     std::cout << "Alright. Run it when you're ready :)" << std::endl;
     return 0;
   }
 }

Apple::Needs();

std::cout << "   @@@@@ " << RED <<  "SSH Ramdisk Maker & Loader" << RESET << " @@@@@" << std::endl;
sleep(2);

std::cout << RED << load::Time() << RESET << " [1] Getting all the components.." << std::endl;
load::GetManifest(identifier, version);
load::Getkeyspage(identifier, version);
load::iBSS(load::tolower(board));
load::iBEC(load::tolower(board));
load::kernel(load::tolower(board));
load::DeviceTree(load::tolower(board));
load::ramdisk(identifier, version);
load::trsc(identifier, version);
std::ifstream check(load::ibss.name);
if(!check) {
  std::cerr << "[!] Something went wrong. Maybe your network connection? Exiting..." << std::endl;
  return -1;
}
else {
  std::cout << "[i] Done!" << std::endl << std::endl;
  sleep(2);
}

std::cout << RED << load::Time() << RESET << " [2] Converting blob to IM4M" << std::endl;
sleep(1);
system((std::string("img4tool -e -s ") + blob + " -m IM4M").c_str());
std::cout << "[!] Done!" << std::endl << std::endl;

std::cout << RED << load::Time() << RESET << " [3] Decrypting " << load::ipsw.ramdisk << "..." << std::endl;
system((std::string("img4 -i ") + load::ipsw.ramdisk + " -o ramdisk.dmg").c_str());
std::cout << "[i] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << " [i] Beginning to patch and sign the components.." << RESET << std::endl << std::endl;

std::cout << RED << load::Time() << RESET << " [4] Decrypting & Patching iBSS.." << RESET << std::endl;
system((std::string("img4 -i ") + load::ibss.name + " -o ibss.raw -k " + load::iBSSIv() + load::iBSSKey() + load::iBSSIV() + load::iBSSKEY()).c_str());
system("kairos ibss.raw ibss.pwn");
system("img4 -i ibss.pwn -o iBSS.img4 -M IM4M -A -T ibss");
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [5] Decrypting & Patching iBEC.." << RESET << std::endl; // 'RED' and 'RESET' s definition is located in "Needs.hpp"
system((std::string("img4 -i ") + load::ibec.name + " -o ibec.raw -k " + load::iBECIV() + load::iBECKEY()).c_str());
system("kairos ibec.raw ibec.pwn -b \"rd=md0 -v\"");
system("img4 -i ibec.pwn -o iBEC.img4 -M IM4M -A -T ibec");
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [6] Decrypting & Patching kernel.." << RESET << std::endl;
system((std::string("img4 -i ") + load::ipsw.Kernel + " -o kernel.raw").c_str());
system("Kernel64Patcher kernel.raw kernel.patched -a");
system("img4tool -c kernel.im4p -t rkrn kernel.patched --compression complzss"); // no need for the diff python script. compression does the job perfectly!
system("img4tool -c KernelCache.img4 -p kernel.im4p");
system("img4 -i KernelCache.img4 -M IM4M");
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [7] Converting DeviceTree to rdtr.." << RESET << std::endl; // rdtr == restore devicetree
system((std::string("img4 -i ") + load::ipsw.Devicetree + " -o DeviceTree.img4 -M IM4M -T rdtr").c_str());
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);

std::cout << RED << load::Time() << RESET << " [8] stitching IM4M to trustcache.." << RESET << std::endl;
system((std::string("img4 -i ") + load::ipsw.trustcache + " -o Trustcache.img4 -M IM4M").c_str());
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);
chdir("..");
mkdir((std::string("Patched") + "_" + identifier + "_" + version).c_str(), S_IRWXU);
chdir((std::string("WD_") + identifier + "_" + version).c_str());
system((std::string("mv Trustcache.img4 ../Patched") + "_" + identifier + "_" + version + "; mv iBEC.img4 ../Patched" + "_" + identifier + "_" + version + "; mv iBSS.img4 ../Patched" + "_" + identifier + "_" + version + "; mv KernelCache.img4 ../Patched" + "_" + identifier + "_" + version + "; mv DeviceTree.img4 ../Patched" + "_" + identifier + "_" + version).c_str());
std::cout << "[i] Done!" << std::endl << std::endl;
sleep(1);
chdir("..");
system("clear");

if(verbose == true) {
  std::cout << "[i] Booting device..." << '\n';
  load::VerboseBoot(version);
  return 0;
}
else if(customlogo == true) {
chdir((std::string("WD_") + identifier + "_" + version).c_str());
system((std::string("img4 -i ") + logo + " -o logo.raw").c_str());
system("img4 -i logo.raw -o logo.img4 -M IM4M -A -T logo");
system((std::string("mv -v logo.img4 ../Patched_") + identifier + "_" + version).c_str());
chdir("..");
load::BootWithCustomLogo(version);
return 0;
}
std::cout << "[i] Patched bootchain now only the ramdisk is left to customise" << std::endl << "Wanna customise the ramdisk yourself or should i? type n to exit & y to proceed: ";
std::cin >> load::ipsw.usrin;
if(load::ipsw.usrin == "n"){
  std::cout << RED << "[note] When you are done customising the ramdisk pack it back to img4 with: 'img4 -i ramdisk.dmg -o ramdisk -M IM4M -A -T rdsk' and move the file to: 'Patched_" << identifier << "_" << version << "' and run this script again. It should automatically goto booting.." << '\n';
  return 0;
}
else if(load::ipsw.usrin == "y"){
chdir((std::string("WD_") + identifier + "_" + version).c_str());
system("hdiutil resize -size 160mb ramdisk.dmg");
load::mount();
chdir("..");
system((std::string("xcrun -sdk iphoneos clang++ -arch arm64 ") + "Stuff/restored_external.cc -o restored_external" + " -std=c++11 -Wno-write-strings").c_str());
system("ldid2 -S restored_external");
system((std::string("mv -v ") + load::ipsw.rdpath + "/usr/local/bin/restored_external " + load::ipsw.rdpath + "/usr/local/bin/restored_external_original").c_str());
system((std::string("mv -v restored_external ") + load::ipsw.rdpath + "/usr/local/bin/restored_external").c_str());
chdir((std::string("WD_") + identifier + "_" + version).c_str());
std::string debs[] = {
"https://apt.bingner.com/debs/550.58/shell-cmds_118-8_iphoneos-arm.deb",
"https://apt.bingner.com/debs/550.58/bash_5.0.3-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/ldid_2:2.0.1-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/make_4.2.1-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/nano_4.5-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/sed_4.5-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/system-cmds_790.30.1-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/tar_1.33-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/unzip_6.0-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/tree_1.8.0-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/grep_3.1-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/img4tool_197-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/550.58/coreutils-bin_8.31-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/550.58/coreutils_8.31-1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/com.bingner.plutil_0.2.1_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/firmware-sbin_0-1_iphoneos-all.deb",
"https://apt.bingner.com/debs/1443.00/launchctl-25_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/dpkg_1.19.7-2_iphoneos-arm.deb",
"https://apt.bingner.com/debs/1443.00/com.bingner.snappy_1.3.0_iphoneos-arm.deb",
"https://cydia.ichitaso.com/debs/Dropbear.deb",
"https://apt.bingner.com/debs/1443.00/openssl_1.1.1i-1_iphoneos-arm.deb",
//"https://apt.bingner.com/debs/1443.00/mobilesubstrate_0.9.7113_iphoneos-arm.deb",
//"http://apt.thebigboss.org/repofiles/cydia/debs2.0/afc2kppless_1.1.5-1.deb"
};
// if you want to add more debs just put the link in the array above and the deb filename in the array below :)
std::string deb[] = {
"shell-cmds_118-8_iphoneos-arm.deb",
"bash_5.0.3-1_iphoneos-arm.deb",
"ldid_2:2.0.1-2_iphoneos-arm.deb",
"make_4.2.1-2_iphoneos-arm.deb",
"nano_4.5-1_iphoneos-arm.deb",
"sed_4.5-1_iphoneos-arm.deb",
"system-cmds_790.30.1-2_iphoneos-arm.deb",
"tar_1.33-1_iphoneos-arm.deb",
"unzip_6.0-1_iphoneos-arm.deb",
"tree_1.8.0-1_iphoneos-arm.deb",
"grep_3.1-1_iphoneos-arm.deb",
"img4tool_197-1_iphoneos-arm.deb",
"coreutils-bin_8.31-1_iphoneos-arm.deb",
"coreutils_8.31-1_iphoneos-arm.deb",
"com.bingner.plutil_0.2.1_iphoneos-arm.deb",
"firmware-sbin_0-1_iphoneos-all.deb",
"launchctl-25_iphoneos-arm.deb",
"dpkg_1.19.7-2_iphoneos-arm.deb",
"com.bingner.snappy_1.3.0_iphoneos-arm.deb",
"Dropbear.deb",
"openssl_1.1.1i-1_iphoneos-arm.deb",
//"mobilesubstrate_0.9.7113_iphoneos-arm.deb",
//"afc2kppless_1.1.5-1.deb"
};
mkdir("bins", S_IRWXU);
mkdir("temp", S_IRWXU);
chdir("temp");

for(auto &i : debs) {
  system((std::string("curl -C - -s -LO ") + i).c_str());
}
for(auto &i : deb) {
std::ifstream dir((i + ".dir"));
if(!dir){
  mkdir((std::string(i) + ".dir").c_str(), S_IRWXU);
}
chdir(((std::string(i) + ".dir").c_str()));
system((std::string("ar -x ../") + i).c_str());
chdir("..");
}
for(auto &i : deb) {
  chdir(((std::string(i) + ".dir").c_str()));
  system((std::string("tar -C ../../bins -xvf data.*")).c_str());
  chdir("..");
}
chdir("..");
std::cout << RED << load::Time() << RESET << " [i] I've put sudo before the rsync command so it copies everything and nothing is left behind so it will ask for your password." << std::endl << "If your not comfortable with it just hit n it will run without sudo and y with sudo: ";
std::cin >> input1;
if(input1 == "y"){
system((std::string("sudo rsync --ignore-existing -avhuK --progress ./bins/ \"") + load::ipsw.rdpath + "/\"").c_str());
}
else if(input1 == "n"){
system((std::string("rsync --ignore-existing -avhuK --progress ./bins/ \"") + load::ipsw.rdpath + "/\"").c_str());
}
system((std::string("hdiutil detach ") + load::ipsw.rdpath).c_str());
std::cout << "[!] Done!" << std::endl << std::endl;
sleep(1);
std::cout << RED << load::Time() << RESET << " [9] Packing ramdisk back.." << std::endl;
system("img4 -i ramdisk.dmg -o ramdisk -M IM4M -A -T rdsk"); // .img4 format = im4p + im4m(shshblob).
system("clear");
system((std::string("mv -v ramdisk ") + "../Patched" + "_" + identifier + "_" + version).c_str());
chdir("..");
}

std::cout << THICKRED << "[!] Keep in Mind. If your system is on ios 15, DO NOT TRY THIS TOOL!! \nIf the rootfs on ios 15 is mounted and edited you'll fuck up and the device will bootloop" << std::endl << std::endl;
std::cout << "[i] Would you like to boot the ramdisk now? y/n: ";
std::cin >> input2;
if(input2 == "y"){
  load::Ramdisk(version);
}
else {
  return 0;
}
} // end main function
