

class Config {
	/* Config file format:
	 * project XXXXXX
	 * \tbuildcmd XXXX XXXX
	 * \truncmd XXXX
	 * #vim: set tw=8 sw=8 ai noet:
	 */
	static const char COMMENTCHAR = '#';
	static const char INDENTATIONCHAR = '\t';
	static const char* CMDBUILD = "buildcmd";
	static const char* CMDRUN = "runcmd";
	static const char* CMDPROJECT = "project";
	const enum item_type = {
		project_name,
		build_command,
		run_command,
		file_mask,
	};
	static struct {
		item_type Type;
		char* Buffer;
	} Item;

}
