#ifndef VCF_STATS_H
#define VCF_STATS_H

#include <stdlib.h>
#include <string.h>

#include <containers/array_list.h>
#include <containers/list.h>
#include <commons/log.h>

#include "vcf_file_structure.h"
#include "vcf_util.h"

/**
 * @file vcf_stats.h
 * @author Cristina Yenyxe Gonzalez Garcia
 * @brief Functions for getting statistics from a VCF file
 * @details This file includes functions for getting statistics from a VCF file. These statistics can 
 * be obtained one per variant or after analyzing the whole file.
 */


/**
 * @brief Statistics global to a VCF file
 * 
 * These are, among others, the number of variants, SNPs and indels it contains, and the mean quality 
 * of the measures.
 **/
typedef struct file_stats {
    int variants_count;         /**< Number of variants of the file. */
    int samples_count;          /**< Number of samples of the file. */
    
    int snps_count;             /**< Number of SNPs of the file. */
    int indels_count;           /**< Number of insertions/deletions of the file. */
    
    int transitions_count;      /**< Number of transitions (A <-> G or C <-> T) of the file. */
    int transversions_count;    /**< Number of transversions (A <-> C, A <-> T, C <-> G, G <-> T) of the file. */
    
    int biallelics_count;       /**< Number of variants with only one alternate allele of the file. */
    int multiallelics_count;    /**< Number of variants with more than one alternate allele of the file. */
    
    int pass_count;             /**< Number of variants that passed a filter. */
    float accum_quality;        /**< Sum of all values of the QUALITY column. */
    float mean_quality;         /**< Mean of all values of the QUALITY column. */
} file_stats_t;


/**
 * @brief Statistics of a variant of a VCF file
 * 
 * These are, among others, the count and frequency of each allele and genotype that can be obtained by 
 * combining them.
 **/
typedef struct variant_stats {
    char *chromosome;           /**< Chromosome of the variant. */
    unsigned long position;     /**< Position of the variant. */
    
    char *ref_allele;           /**< Reference allele of the variant. */
    char **alternates;          /**< List of alternate alleles of the variant. */
    
    int num_alleles;            /**< Number of alleles of the variant (1 reference + N alternates). */
    int *alleles_count;         /**< Times each allele has been counted. */
    int *genotypes_count;       /**< Times each possible genotype has been counted. */
    float *alleles_freq;        /**< Frequency of each allele in relation to the total. */
    float *genotypes_freq;      /**< Frequency of each genotype in relation to the total. */
    
    int missing_alleles;        /**< Number of alleles whose information is missing. */
    int missing_genotypes;      /**< Number of genotypes with at least one allele missing. */
} variant_stats_t;


/* ********************************
 * Initialization and destruction *
 * ********************************/

/**
 * @brief Allocates memory for a file_stats_t
 * @return A new file_stats_t structure
 **/
file_stats_t *file_stats_new();

/**
 * @brief Frees memory associated to a file_stats_t structure.
 * @param file_stats The structure to be freed
 */
void file_stats_free(file_stats_t *file_stats);


/**
 * @brief Initializes a variant_stats_t structure mandatory fields.
 * @return A new variant_stats_t structure
 * 
 * Initializes a variant_stats_t structure mandatory fields, which are its chromosom, position and 
 * reference allele.
 */
variant_stats_t *variant_stats_new(char *chromosome, unsigned long position, char *ref_allele);

/**
 * @brief Free memory associated to a variant_stats_t structure.
 * @param variant_stats The structure to be freed
 */
void variant_stats_free(variant_stats_t *variant_stats);



/* ******************************
 *           Execution          *
 * ******************************/

/**
 * @brief Given a list of variants, gets their statistics and also the ones that apply to the VCF file
 *
 * @param variants The list of variants whose statistics will be got
 * @param num_variants The number of variants
 * @param output_list [out] The list where the statistics per variant will be stored
 * @param file_stats [in,out] The statistics of the VCF file
 * @return Whether the statistics were successfully retrieved
 * 
 * Given a list of variants, gets their statistics and also the ones that apply to the VCF file. The statistics 
 * per variant are queued in the output_list argument, and the statistics of the whole file are stored in the 
 * file_stats structure.
 **/
int get_variants_stats(vcf_record_t **variants, int num_variants, list_t *output_list, file_stats_t *file_stats);

/**
 * @brief Given the statistics of a file (supposedly not full-processed), updates the value of its statistics
 *
 * @param variants_count Number of new variants found
 * @param samples_count Number of samples
 * @param snps_count Number of new SNPs found
 * @param transitions_count Number of new transitions found
 * @param transversions_count Number of new transversions found
 * @param indels_count Number of new insertions/deletions found
 * @param biallelics_count Number of new biallelic variants found
 * @param multiallelics_count Number of new multiallelic variants found
 * @param pass_count Number of new variants that passed a filter found
 * @param accum_quality Sum of all new values of the QUALITY column found
 * @param stats [in,out] The statistics of the VCF file
 * 
 * Given the statistics of a file (supposedly not full-processed), updates the value of its statistics. The variants, 
 * samples, SNPs, transition, transversion, indels, biallelic, multiallelic and PASS count will be accumulated to the 
 * previous values. The accumulated quality will also be summed, and as a result its mean will be recalculated.
 **/
void update_file_stats(int variants_count, int samples_count, int snps_count, int transitions_count, int transversions_count, 
                       int indels_count, int biallelics_count, int multiallelics_count, int pass_count, float accum_quality, 
                       file_stats_t *stats);


#endif
