void cleanup() {

	destroy_bitmap (buffer);
	release_screen();

	for (it = entity.begin(); it != entity.end(); ++it)
		delete *it;
	entity.clear();
}
