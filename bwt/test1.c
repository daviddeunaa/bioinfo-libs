#include "test1.h"


void main(int argc, char *argv[]) {

  char *input_filename, *index_dirname;

  if (argc != 3) {
    printf("Error.\n");
    printf("Usage: %s input-filename index-dirname\n", argv[0]);
    exit(-1);
  }

  input_filename = argv[1];
  index_dirname = argv[2];

  // initializations
  initReplaceTable();

  printf("input_filename = %s, index_dirname = %s\n", input_filename, index_dirname);

  bwt_index_t *bwt_index = bwt_index_new(index_dirname);

  printf("**** index_dirname = %s\n", bwt_index->dirname);


  array_list_t *mappings = array_list_new(100000, 0.25f, COLLECTION_MODE_SYNCHRONIZED);
  fastq_read_t *fq_read;
  unsigned int num_mappings;



  fq_read = fastq_read_new("@read_0 2L_11177498_11177561/1", 
			   "TATTTATTCGCAAATGCATAACTTGAAAGGCTATTTTGTAGATATTAAATGCAACTAATTTGCC", 
			   "################################################################");

  fq_read = fastq_read_new("@read_1 2L_253017_253081/1", 
			   "CTTGTCGAAGTACTGCCGCTTGGTTCGGTTGGGATCCTCAGCCTCGCGCACCTTTTCGGCAATCT",
			   "#################################################################");

  fq_read = fastq_read_new("@read_2 3R_15260115_15260182/1", 
                           "CTTGTCGAAGTACTGCCGCTTGGTTCGTTGGGATCCTCAGCCTCGCGCACCTTTTCGGCAATCT",
			   "################################################################");

  num_mappings = bwt_map_read_cpu(fq_read, NULL, bwt_index, mappings);
  printf("num_mappings %i found for %s\n", num_mappings, fq_read->id);

  alignment_t *mapping;
  for (int i = 0; i < array_list_size(mappings); i++) {
    mapping = (alignment_t *) array_list_get(i, mappings);
    printf("%s %s %s\n", mapping->query_name, mapping->sequence, mapping->cigar);
  }

  fastq_read_free(fq_read);
  /*
  fq_read = fastq_read_new("@read_4 2L_253017_253081/1",
			   "CTTGTCGAAGTACTGCCGCTTGGTTCGTTGGGATCCTCAGCCTCGCGCACCTTTTCGGCAATCT",
			   "################################################################");

  num_mappings = bwt_single_read_cpu(fq_read, NULL, bwt_index, NULL);
  printf("num_mappings %i found for %s\n", num_mappings, fq_read->id);
  fastq_read_free(fq_read);
  */
  /*


  fq_read = fastq_read_new("@read_5 2L_11177498_11177561/1",
			   "TATTTATTCGCAAATCGCATAACTTGAAAGGCTATTTTGTAGATATTAAATGCAACTAATTTGCC",
			   "#################################################################");

  */
  


  /*
  int num_reads;
  unsigned long batch_size = 64 * 1024;
  fastq_batch_t *fq_batch;

  // open FastQ files
  fastq_file_t *fq_file = fastq_fopen(input_filename);

  fastq_read_t read;


  while (1) {

    fastq_fread(
    //fq_batch = fastq_batch_new();
    //fastq_batch_init(fq_batch, batch_size);

    //    num_reads = fastq_fread_batch_max_size(fq_batch, batch_size, fq_file);

    printf("num reads = %i\n", num_reads);

    // if no reads, free memory and go out...
    if (num_reads == 0) {
      fastq_batch_free(fq_batch);
      break;
    }


    


    fastq_batch_free(fq_batch);
  }

  // close FastQ file and free memory
  fastq_fclose(fq_file);
  */


  bwt_index_free(bwt_index);
}