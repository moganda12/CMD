{ pkgs }: {
	deps = [
   pkgs.neofetch
   pkgs.systemd
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
		pkgs.libao
		pkgs.gmp
	];
}