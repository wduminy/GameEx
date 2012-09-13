/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include <stddef.h>
#include <stdio.h>
#include <stdexcept>
#include <stdlib.h>
#include <iostream>
#include <sstream>

namespace systemex {
	struct MemoryBlock {
			long state;
			struct MemoryBlock *next;
			struct MemoryBlock *prev;
			size_t size;
			const char *file;
			int line;
			int padding; // Make it an even 16 byte length (total size=32 bytes in 32-bit mode).
	};

	struct MemoryBlock * blockAtEnd = 0;
	long ALLOCATED = 12345678L;
	long FREED = 8754321L;

	void *track_malloc(size_t size, const char *file, int line) {
		struct MemoryBlock *blockToAllocate = static_cast<MemoryBlock*>(malloc(
				size + sizeof(*blockToAllocate)));
		if (blockToAllocate == 0)
			throw std::runtime_error("could not allocate memory");
		blockToAllocate->state = ALLOCATED;
		blockToAllocate->file = file;
		blockToAllocate->line = line;
		blockToAllocate->size = size;
		blockToAllocate->prev = 0;
		blockToAllocate->next = blockAtEnd;
		if (blockAtEnd)
			blockAtEnd->prev = blockToAllocate;
		blockAtEnd = blockToAllocate;
		// return the memory allocated at the end of the block
		return (void *) &blockToAllocate[1];
	}

	void track_free(void *ptr, const char *, int) {
		if (!ptr)
			return;
		else {
			struct MemoryBlock *blockToFree = &((struct MemoryBlock *) (ptr))[-1];
			if (blockToFree->state != ALLOCATED)
				throw std::runtime_error("Cannot free unallocated block");
			blockToFree->state = FREED;
			// change 'end block' if needed
			if (blockToFree == blockAtEnd)
				blockAtEnd = blockAtEnd->prev;
			// Unlink it.
			if (blockToFree->next)
				blockToFree->next->prev = blockToFree->prev;
			if (blockToFree->prev)
				blockToFree->prev->next = blockToFree->next;
			free(blockToFree);
		}
	}

	std::string memoryLeakReport() {
#ifdef FIND_LEAKS
		std::string result;
		result += "*** Allocation list start ***\n";
		if (!blockAtEnd) {
			result += ">>> EMPTY <<<\n";
		} else {
			struct MemoryBlock *mb;
			for (mb = blockAtEnd; mb; mb = mb->next) {
				std::ostringstream temp;
				temp << mb->line;
				result += "leak at file ";
				result += mb->file;
				result += " line ";
				result += temp.str().c_str();
				result += "\n";
			}
		}
		result += "*** Allocation list end ***\n";
		return result;
#else
		return "Memory leak detection is off";
#endif
	}
	;

}
