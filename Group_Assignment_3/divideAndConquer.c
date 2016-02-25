#include "myLibrary.h"

int main(){
	int array[] = {386521, -426105, -51327, -479314, 266939, -483585, 5174, -26325, -174507, 420884, 16180, 349704, 60431, -345921, 402022, 139333, -132008, 72744, -102568, -46460, 450996, -371577, -488895, 58007, -235653, 297566, -325213, -257872, 93030, 3737, -166953, -32082, -149683, -359092, -414417, 78794, 61943, 223186, 79294, -170009, -32876, 412168, -199555, 488317, 432080, 448046, 98065, 304298, -183989, -72475, -148109, 397536, 482222, 374665, -389837, -220047, -474078, -345931, -180385, -136260, 427174, 417819, 318218, 83008, 239618, 496199, 195442, -346491, -182687, -12742, -315836, 129197, -165305, 13224, -63483, 461599, 118305, -136678, 252502, 304408, 192900, -384603, -49832, -225063, -388805, 30286, -58632, 224370, -212093, -214778, 227535, -263757, 430684, 318599, 21355, 52281, -73725, -486957, -118220, 62683};
	int arrayLength = (int)sizeof(array)/sizeof(int);

	printf("%d unit\n", arrayLength);

	struct closestToZero result;
	result = find(array, 0, arrayLength-1);

	printf("Result: cloest: %d, from %d to %d\n", result.closest, result.i, result.j);

	return 0;
}

struct closestToZero find(int *array, int i, int j){
	struct closestToZero suffix;
	struct closestToZero prefix;
	struct closestToZero combine;
	struct closestToZero result;
	int temp;
	int divide;

	if((j-i) == 1){
		temp = abs(array[i] + array[j]);
		if(temp < abs(array[i]) && temp < abs(array[j])){
			result.i = i;
			result.j = j;
			result.closest = temp;
		}else if(abs(array[i]) > abs(array[j])){
			result.i = j;
			result.j = j;
			result.closest = abs(array[j]);
		}else{
			result.i = i;
			result.j = i;
			result.closest = abs(array[i]);
		}
		return result;
	}else if(i == j){
		result.i = i;
		result.j = j;
		result.closest = abs(array[i]);
		return result;
	}

	divide = (i + j)/2;
	//printf("Divide in find : %d i= %d j = %d\n", divide, i, j);
	suffix = find(array, i, divide);
	prefix = find(array, divide + 1, j);
	combine = methodThree(array, i, j);

	return min(suffix, prefix, combine);
}

struct closestToZero min(struct closestToZero suffix, struct closestToZero prefix, struct closestToZero combine){
	struct closestToZero temp;
	if(suffix.closest < prefix.closest)
		temp = suffix;
	else
		temp = prefix;

	if(combine.closest < temp.closest){
		//printf("%d\n", combine.closest);
		return combine;
	}else{
		//printf("%d\n", temp.closest);
		return temp;
	}
}

struct closestToZero methodOne(int *array, int start, int end){
	//printf("i = %d, j = %d", start, end);
	int divide;
	struct closestToZero temp, smallest;
	int i, j;
	struct method *suffixSum, *prefixSum;
	divide = (start + end + 1)/2;

	//printf("Divide: %d\n", divide);

	suffixSum = malloc((divide - start)*sizeof(struct method*));
	prefixSum = malloc((end - (divide - 1))*sizeof(struct method*));

	//sum the two arrays
	for(i = divide - 1, j = divide - start - 1; i >= start; i--, j--){
		if(i == divide - 1){
			suffixSum[j].sum = array[i];
			suffixSum[j].position = i;
		}
		else{
			suffixSum[j].sum = suffixSum[j + 1].sum + array[i];
			suffixSum[j].position = i;
		}

		//printf("%d ", suffixSum[i].sum);
	}

	//printf("\n");

	for(i = 0; i < end - (divide - 1); i++){
		if(i == 0){
			prefixSum[i].sum = array[divide + i];
			prefixSum[i].position = divide + i;
		}
		else{
			prefixSum[i].sum = prefixSum[i - 1].sum + array[divide + i];
			prefixSum[i].position = divide + i;
		}

		//printf("%d ", prefixSum[i].sum);
	}

	//printf("\n");

	smallest.closest = 100000000;
	for(i = 0; i < divide - start; i++){
		for(j = 0; j < end - (divide - 1); j++){
			temp.closest = abs(suffixSum[i].sum + prefixSum[j].sum);
			temp.i = suffixSum[i].position;
			temp.j = prefixSum[j].position;

			if(temp.closest < smallest.closest){
				smallest = temp;
			}
		}
	}
	//printf(" Smallest from method 1: %d i = %d, j = %d\n", smallest.closest, smallest.i, smallest.j);

	//free(prefixSum);
	//free(suffixSum);

	return smallest;
}

//method 2
//struct closestToZero methorTwo(int *array, int start, int end){
//}

//method 3
struct closestToZero methodThree(int *array, int start, int end){
	//printf("start: %d, end: %d\n", start, end);
	int divide;
	struct closestToZero temp, smallest;
	int i, j, k;
	struct method *suffixSum, *prefixSum, *combine;
	divide = (start + end + 1)/2;

	//printf("Divide: %d\n", divide);

	suffixSum = malloc((divide - start)*sizeof(struct method));
	prefixSum = malloc((end - (divide - 1))*sizeof(struct method));
	combine = malloc((end - start + 1)*sizeof(struct method));

	//sum the two arrays
	for(i = divide - 1, j = divide - start -1, k = 0; i >= start; i--, j--, k++){
		if(i == divide - 1){
			suffixSum[j].sum = array[i];
			suffixSum[j].position = i;
		}
		else{
			suffixSum[j].sum = suffixSum[j + 1].sum + array[i];
			suffixSum[j].position = i;
		}

		combine[k].sum = suffixSum[j].sum;
		combine[k].position = suffixSum[j].position;
		//printf("%d i = %d ", suffixSum[i].sum, i);
	}

	//printf(" ==> suffixSum\n");

	for(i = 0; i < end - (divide - 1); i++, k++){
		if(i == 0){
			prefixSum[i].sum = array[divide + i];
			prefixSum[i].position = divide + i;
		}
		else{
			prefixSum[i].sum = prefixSum[i - 1].sum + array[divide + i];
			prefixSum[i].position = divide + i;
		}

		prefixSum[i].sum = prefixSum[i].sum;

		combine[k].sum = -prefixSum[i].sum;
		combine[k].position = prefixSum[i].position;
		//printf("%d ", prefixSum[i].sum);
	}

	//printf("==> prefixSum, Combine: ");

	sort(combine, end - start + 1);
	/*for(i = 0; i < end - start + 1; i++){
		printf("%d ", combine[i].sum);
	}
	printf("\n");*/

	smallest.closest = 1000000;

	for(i = 0; i < end - start; i++){
		temp.closest = abs(combine[i].sum - combine[i+1].sum);
		temp.i = combine[i].position;
		temp.j = combine[i+1].position;

		if(temp.closest < smallest.closest){
			smallest = temp;
		}
	}

	//printf("check 1 smallest: %d\n", smallest.closest);

	//free(combine);
	//free(prefixSum);
	//free(suffixSum);

	return smallest;
}

void sort(struct method *combine, int length){
	struct method temp;
	int i, j;

	for(i = 1; i < length; i++){
		for(j = i; j > 0; j--){
			if(combine[j].sum < combine[j-1].sum){
				temp = combine[j-1];
				combine[j-1] = combine[j];
				combine[j] = temp;
			}
		}
	}
}