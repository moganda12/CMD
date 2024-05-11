{ pkgs }: {
	deps = [
   pkgs.systemd
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
		pkgs.libao
	];
}