#include <fstream>
#include <iostream>
#include <string>
#include "SongLibrary.h"

//////////////////DO NOT CHANGE THIS CODE////////////////////////

//Construct a Song object with values
Song::Song(const std::string& title, const std::string& artist, 
     const std::string& album, unsigned int year){
	this->title = title;
	this->artist = artist;
	this->album = album;
	this->year = year;
	used = false;
	this->song_group_ptr = NULL;
}

//Song equality doesn't depend on pointers or being used
bool operator== (const Song& s1, const Song& s2){
	return (s1.getYear() == s2.getYear()) && (s1.getTitle() == s2.getTitle())
          && (s1.getAlbum() == s2.getAlbum()) && (s1.getArtist() == s2.getArtist());
}

//Modifier for group pointer
void Song::updateSongGroupPointer(SongGroupNode* ptr){
	if(ptr){
		//Check that ptr is actually for this Song.
		assert(ptr->m_song->value == *this); 
		song_group_ptr = ptr;
	}
	else{
		song_group_ptr = NULL;
	}
}

//Make output easier by overload operator<<
std::ostream& operator<< (std::ostream& ostr, const Song& song){
	if(!song.getSongGroupPointer()){
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
	       << song.getAlbum() << "\" (" << song.getYear() << ")" << std::endl;
	}
	else{
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
		<< song.getAlbum() << "\" (" << song.getYear() << ")" << " in Song Group "
		<< song.getSongGroupPointer()->id << std::endl;
	}
	return ostr;
}

/////////////////////////YOUR CODE STARTS HERE/////////////////////////////////


//////////////////////////////GROUP FUNCTIONS/////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are searching for
BEHAVIOR:
None
RETURN:
NULL if no list matches the ID, otherwise returns the GroupListNode* containing
the head of the list with ID id.
*/
GroupListNode* GroupExists(GroupListNode* song_groups, GroupID id){
	GroupListNode* song_group = song_groups;
	while (song_group != NULL) {
		if(song_group->value->id == id) {
			return song_group;
		}
		song_group = song_group->ptr;
	}
	return NULL;
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are creating
BEHAVIOR:
Add a new song group to song_groups, give the new song group ID id. Order of
song groups is not guaranteed.
RETURN:
None
*/
void MakeGroup(GroupListNode*& song_groups, GroupID id){
	GroupListNode* new_song_group = new GroupListNode;
	new_song_group->value = new SongGroupNode;

	//initialize the new group's values to the appropriate values 

	new_song_group->value->id = id;
	new_song_group->value->m_song = NULL;
	new_song_group->value->next_song_ptr = NULL;
	new_song_group->value->prev_song_ptr = NULL;
	new_song_group->value->prev_by_year_ptr = NULL;
	new_song_group->value->next_by_artist_ptr = NULL;

	//rearrange the pointers in song_groups

	new_song_group->ptr = song_groups;
	song_groups = new_song_group;
}

/*
ARGUMENTS:
-group_head points to the head of a song group
-song is the Song we are checking the group for
BEHAVIOR:
None
RETURN:
If song is found in the group return true. If song is not found,
return false.
*/
bool SongInGroup(SongGroupNode* group_head, const Song& song){
	SongGroupNode* node = group_head;
	while (node != NULL) {
		if(node->m_song->value == song) {
			return true;
		}
		node = node->next_song_ptr;
	}
	return false;
}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to add.
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Modify the group pointed to by list_ptr to reflect that the song is in the group.
This may change the length of the list. Modifies the Song's members as well.
RETURN:
None
*/
void AddToGroup(LibraryNode* library_ptr, GroupListNode* list_ptr){
	
	SongGroupNode* node = list_ptr->value;
	if (node->m_song != NULL) {                      //if the list isn't empty, read through it to the last node
		SongGroupNode* prev_song_by_year = NULL;
		while (true) { 
			if (node->m_song->value.getYear() < library_ptr->value.getYear()) {
				prev_song_by_year = node;
			}
			if (node->next_song_ptr == NULL) {
				break;
			}
			else {
			node = node->next_song_ptr;
			}
		}

		//create new SongGroupNode
		SongGroupNode* new_song = new SongGroupNode;
		new_song->next_song_ptr = NULL;                 //this new node is at the end
		new_song->m_song = library_ptr;                 //new node's song is the provided library_ptr
		new_song->id = list_ptr->value->id;             //new node's id is the id of the group
		new_song->prev_song_ptr = node;                 //new node's previous song is the previous final song
		node->next_song_ptr = new_song;                 //previous final song's next song is the new song
		new_song->prev_by_year_ptr = prev_song_by_year;
		new_song->next_by_artist_ptr = NULL;



		new_song->m_song->value.setUsedInGroup(true);
		new_song->m_song->value.updateSongGroupPointer(new_song);

		node = new_song->prev_song_ptr;
		while(node != NULL) {
			if (node->m_song->value.getArtist() == new_song->m_song->value.getArtist()) {
				node->next_by_artist_ptr = new_song;
			}
			node = node->prev_song_ptr;
		}
	}
	else {
		node->m_song = library_ptr;                     //if the group is empty, we just set m_song to the library_ptr
		node->m_song->value.setUsedInGroup(true);
		node->m_song->value.updateSongGroupPointer(node);
	}

}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Modifies the Song's member variables to reflect that it is not in a group. Modifies the group
so that the Song is not in the group. This function may cause song_groups to be modified as well.
RETURN:
None
*/
void RemoveFromGroup(LibraryNode* library_ptr, GroupListNode* song_groups){

	GroupListNode* song_group = song_groups;
	while (true) {
		if (SongInGroup(song_group->value, library_ptr->value)) {
			break;
		}
		song_group = song_group->ptr;
		if (song_group == NULL) {
			std::cerr << "That song wasn't found in any groups." << std::endl;
			return;
		}
	}

	SongGroupNode* deleted_song = library_ptr->value.getSongGroupPointer();
	SongGroupNode* node = song_group->value;

	while(node != NULL) {
		if (node->next_by_artist_ptr == deleted_song) {
			node->next_by_artist_ptr = deleted_song->next_by_artist_ptr;
		}
		if (node->prev_by_year_ptr == deleted_song) {
			node->prev_by_year_ptr = NULL;
		}
		if (node == deleted_song) {
			if (node->prev_song_ptr) {
				node->prev_song_ptr->next_song_ptr = node->next_song_ptr;
			}
			if (node->next_song_ptr){
				node->next_song_ptr->prev_song_ptr = node->prev_song_ptr;
			}
		}
		node = node->next_song_ptr;
	}

	if (song_group->value == deleted_song) {
		if (deleted_song->next_song_ptr != NULL) {
			song_group->value = deleted_song->next_song_ptr;
			delete deleted_song;
		}
		else {
			deleted_song->next_song_ptr = NULL;
			deleted_song->m_song = NULL;
			deleted_song->prev_song_ptr = NULL;
			deleted_song->prev_by_year_ptr = NULL;
			deleted_song->next_by_artist_ptr = NULL;
		}

	}
	else {
		delete deleted_song;
	}


	library_ptr->value.setUsedInGroup(false);
	library_ptr->value.updateSongGroupPointer(NULL);
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr1 contains to the song group head of Group A
-list_ptr2 contains the song group head of Group B
BEHAVIOR:
Modify Group A so that at the end of its songs, the songs of Group B are added in the same
order they were found in Group B. Remove the now-empty Group B.
RETURN:
None
*/
void CombineGroups(GroupListNode*& song_groups, GroupListNode*& list_ptr1, GroupListNode*& list_ptr2){

	SongGroupNode* list_2_node = list_ptr2->value;
	while (list_2_node != NULL) {
		AddToGroup(list_2_node->m_song, list_ptr1);
	}

	RemoveGroup(song_groups, list_ptr2);


}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr is the head of the song group we want to remove.
BEHAVIOR:
Remove the group from the list of song groups. No Song objects should remain associated with the
group, and the function should modify Song member variables appropriately to reflect this. Any
memory associated with the group must be released.
RETURN:
None
*/
void RemoveGroup(GroupListNode*& song_groups, GroupListNode* list_ptr){
	if (song_groups->ptr == NULL) {
		song_groups = NULL;
	}
	else if (song_groups == list_ptr) {
		song_groups = list_ptr->ptr;
	}
	else {
		GroupListNode* group_node = song_groups;
		while (group_node != NULL) {
			if (group_node->ptr == list_ptr) {
				group_node->ptr = list_ptr->ptr;
			}
		}
	}

	SongGroupNode* node;
	SongGroupNode* tmp_node = list_ptr->value;
	while (tmp_node != NULL) {
		node = tmp_node;
		if (node->m_song != NULL) {
			node->m_song->value.setUsedInGroup(false);
			node->m_song->value.updateSongGroupPointer(NULL);
		}
		tmp_node = node->next_song_ptr;
		delete node;
	}
	delete list_ptr; //might have some leaks here woooo
}

/*
ARGUMENTS:
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Recompute all prev_by_year_ptr members in the song group pointed to by
list_ptr.
RETURN:
None
*/
void RecomputePrevByYear(GroupListNode* list_ptr){
	SongGroupNode* node = list_ptr->value;
	unsigned int year = node->m_song->value.getYear();
	while (node != NULL) {
		SongGroupNode* second_node = node->next_song_ptr;
		while (second_node != NULL) {
			if (year < second_node->m_song->value.getYear()) {
				second_node->prev_by_year_ptr = node;
			}
			second_node = second_node->next_song_ptr;
		}
		node = node->next_song_ptr;
	}
}

////////////////////////////PRINT FUNCTIONS////////////////////////////////////

/*
ARGUMENTS:
-library_head points to the first LibraryNode* of the song library
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the library in the order it was added.
RETURN:
None
*/
void PrintLibrary(LibraryNode* library_head,std::ostream& print_file){
	LibraryNode* node = library_head;
	unsigned int song_counter = 0;
	while (node != NULL) {
		print_file << node->value;
		node = node->ptr;
		song_counter++;
	}
	print_file << "Library contains " << song_counter << " song(s)" << std::endl;
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to print
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the group in the order it was added.
RETURN:
None
*/
void PrintGroupSequential(SongGroupNode* group_head,std::ostream& print_file){
	SongGroupNode* node = group_head;
	unsigned int song_counter = 0;
	print_file << "List ID " << node->id << " contains:" << std::endl;
	while (node != NULL && node->m_song != NULL) {
		print_file << (node->m_song->value);
		node = node->next_song_ptr;
		song_counter++;
	}
	print_file << song_counter << " song(s)" << std::endl;
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the prev_by_year_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupRewind(SongGroupNode* group_ptr,std::ostream& print_file){
	SongGroupNode* node = group_ptr;
	unsigned int song_counter = 0;
	print_file << "Rewind list for " << node->m_song->value.getYear() << std::endl;
	while (node != NULL) {
		song_counter++;
		print_file << "   #" << song_counter << ": " << node->m_song->value;
		node = node->prev_by_year_ptr;
	}
	print_file << "Rewind list contained " << song_counter << " song(s)." << std::endl;
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the next_by_artist_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupMarathon(SongGroupNode* group_ptr,std::ostream& print_file){
	unsigned int song_counter = 0;
	SongGroupNode* node = group_ptr;
	print_file << "Marathon list for " << node->m_song->value.getArtist() << std::endl;
	while (node != NULL) {
		song_counter++;
		print_file << "   #" << song_counter << ": " << node->m_song->value;
		node = node->next_by_artist_ptr;
	}
	print_file << "Marathon list contained " << song_counter << " song(s)." << std::endl;
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to analyze
-print_file points to an ostream to print output to
BEHAVIOR:
Find the rewind list with the most songs in the song group pointed to by group_head.
Print the rewind list and the song count to print_file.
RETURN:
None
*/
void PrintLongestGroupRewind(GroupListNode* group_head,std::ostream& print_file){
	SongGroupNode* first_node_in_longest_chain = NULL;
	unsigned int longest_chain_length = 0;

	SongGroupNode* node = group_head->value;
	unsigned int song_counter = 0;

	while (node != NULL) {

		SongGroupNode* chain_link_node = node->prev_by_year_ptr;

		while (chain_link_node != NULL) {
			song_counter++;
			chain_link_node = chain_link_node->prev_by_year_ptr;
			if (song_counter > longest_chain_length) {
				longest_chain_length = song_counter;
				first_node_in_longest_chain = node;
			}
		}
		song_counter = 0;
		node = node->next_song_ptr;
	}

	node = first_node_in_longest_chain;
	if (node != NULL) {
		print_file << "Maximum size rewind list is for year " << first_node_in_longest_chain->m_song->value.getYear() << std::endl;
	}
	else {
		print_file << "No rewind list found" << std::endl;
	}
	while (node != NULL) {
		song_counter++;
		print_file << "   #" << song_counter << ": " << node->m_song->value;
		node = node->prev_by_year_ptr;
	}
	print_file << "Longest rewind list contained " << song_counter << " song(s)." << std::endl;
}

///////////////////////////////CLEANUP/////////////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Delete every group. Do not leak. Modifies all groups and the song_groups list.
Update every Song to reflect not being in a group.
RETURN:
None
*/
void DeleteAllSongGroups(GroupListNode*& song_groups){
	GroupListNode* node = song_groups;
	GroupListNode* tmp_node = song_groups;
	while (tmp_node != NULL) {
		node = tmp_node;
		tmp_node = node->ptr;
		RemoveGroup(song_groups, node);
	}
}